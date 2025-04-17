#pragma once

#include <queue>
#include <functional>
#include <memory>
#include <mutex>
#include <thread>
#include <unordered_map>
#include <vector>
#include <chrono>
#include <condition_variable>
#include <algorithm>

class RenderQueue {
public:
    using RenderCallback = std::function<void(ImDrawList*)>;
    using ComputeCallback = std::function<RenderCallback()>;

    RenderQueue() {
        running = true;
        calculationThread = std::thread(&RenderQueue::ProcessQueue, this);
    }

    ~RenderQueue() {
        Stop();
    }

    void AddTask(const std::string& id, float deltaTime, ComputeCallback computeCallback) {
        std::lock_guard<std::mutex> lock(tasksMutex);
        tasks.push_back({ id, std::move(computeCallback), deltaTime });
    }

    void RemoveTask(const std::string& id) {
        std::lock_guard<std::mutex> lock(tasksMutex);
        for (auto& task : tasks) {
            if (task.id == id) {
                task.active = false;
            }
        }
    }

    void Render() {
        std::unique_lock<std::mutex> lock(resultsMutex);
        auto* drawList = ImGui::GetBackgroundDrawList();
        for (const auto& [id, renderFunc] : frontBuffer) {
            if (renderFunc) {
                renderFunc(drawList);
            }
        }

        if (cv.wait_for(lock, std::chrono::milliseconds(1), [this] { return bufferReady.load(std::memory_order_relaxed); })) {
            frontBuffer = std::move(backBuffer);
            bufferReady.store(false, std::memory_order_relaxed);
            cv.notify_one();
        }
    }

    void Stop() {
        running = false;
        cv.notify_all();
        if (calculationThread.joinable()) {
            calculationThread.join();
        }
    }

private:
    struct RenderTask {
        std::string id;
        ComputeCallback computeCallback;
        std::atomic<bool> active;
        float deltaTime{ 0.033f };

        RenderTask() : active(true) {}
        RenderTask(std::string id_, ComputeCallback&& cb, float dt)
            : id(std::move(id_)), computeCallback(std::move(cb)), active(true), deltaTime(dt) {
        }
        RenderTask(RenderTask&& other) noexcept
            : id(std::move(other.id)),
            computeCallback(std::move(other.computeCallback)),
            active(other.active.load(std::memory_order_relaxed)),
            deltaTime(other.deltaTime) {
        }
        RenderTask& operator=(RenderTask&& other) noexcept {
            if (this != &other) {
                id = std::move(other.id);
                computeCallback = std::move(other.computeCallback);
                active.store(other.active.load(std::memory_order_relaxed), std::memory_order_relaxed);
                deltaTime = other.deltaTime;
            }
            return *this;
        }
        RenderTask(const RenderTask&) = delete;
        RenderTask& operator=(const RenderTask&) = delete;
    };

    std::vector<RenderTask> tasks;
    std::mutex tasksMutex;
    std::unordered_map<std::string, RenderCallback> frontBuffer;
    std::unordered_map<std::string, RenderCallback> backBuffer;
    std::mutex resultsMutex;
    std::condition_variable cv;
    std::atomic<bool> running{ true };
    std::atomic<bool> bufferReady{ false };
    std::thread calculationThread;

    void ProcessQueue() {
        static float monitorRefreshRate = []() {
            HDC hdc = GetDC(nullptr);
            if (hdc) {
                int refreshRate = GetDeviceCaps(hdc, VREFRESH); 
                ReleaseDC(nullptr, hdc);
                if (refreshRate > 0) {
                    return static_cast<float>(refreshRate);
                }
            }
            return 60.0f; 
            }();

        float targetFrameTime = 1.0f / monitorRefreshRate;

        while (running) {
            auto frameStart = std::chrono::high_resolution_clock::now();

            std::vector<RenderTask> activeTasks;
            float minDeltaTime = targetFrameTime; 
            {
                std::lock_guard<std::mutex> lock(tasksMutex);
                activeTasks.reserve(tasks.size());
                for (auto it = tasks.begin(); it != tasks.end();) {
                    if (it->active) {
                        activeTasks.push_back(std::move(*it));
                        minDeltaTime = min(minDeltaTime, it->deltaTime); 
                        ++it;
                    }
                    else {
                        it = tasks.erase(it);
                    }
                }
            }

            std::unordered_map<std::string, RenderCallback> tempResults;
            for (auto& task : activeTasks) {
                if (task.computeCallback) {
                    try {
                        tempResults[task.id] = task.computeCallback();
                    }
                    catch (...) {
                        tempResults[task.id] = [](ImDrawList*) {};
                    }
                }
            }

            {
                std::unique_lock<std::mutex> lock(resultsMutex);
                cv.wait(lock, [this] { return !bufferReady.load(std::memory_order_relaxed) || !running; });
                if (!running) break;

                backBuffer = std::move(tempResults);
                bufferReady.store(true, std::memory_order_relaxed);
                lock.unlock();
                cv.notify_one();
            }

            auto frameEnd = std::chrono::high_resolution_clock::now();
            auto frameTime = std::chrono::duration<float>(frameEnd - frameStart).count();
            if (frameTime < targetFrameTime) {
                std::this_thread::sleep_for(std::chrono::duration<float>(targetFrameTime - frameTime));
            }
        }
    }
};