#pragma once

class Cheat {
public:
    Cheat(const std::string& name) : name_(name), enabled_(false) {}
    virtual ~Cheat() = default;

    virtual void Enable() { enabled_ = true; }
    virtual void Disable() { enabled_ = false; }
    virtual void Update() = 0;
    virtual void Draw() = 0;

    virtual void OnEnable() = 0;
    virtual void OnDisable() = 0;

    std::string GetName() const { return name_; }
    bool IsEnabled() const { return enabled_; }

protected:
    std::string name_;
    bool enabled_;
};

#include "cheats/AntiStamina.h"

class CheatManager {
public:
    CheatManager() = default;
    ~CheatManager() = default;

    static void Uninitialise();
    static void Initialise();
    static void Process();

    template<typename T>
    void AddCheat(const std::string& name) {
        static_assert(std::is_base_of<Cheat, T>::value, "T must inherit from Cheat");
        cheats_.push_back(std::make_unique<T>(name));
    }

    void EnableCheat(const std::string& name) {
        auto cheat = FindCheat(name);
        if (cheat) {
            cheat->Enable();
            cheat->OnEnable();
        }
    }

    void DisableCheat(const std::string& name) {
        auto cheat = FindCheat(name);
        if (cheat) {
            cheat->Disable();
            cheat->OnDisable();
        }
    }

    void ToggleCheat(const std::string& name) {
        auto cheat = FindCheat(name);
        if (cheat) {
            if (cheat->IsEnabled()) 
            {
                cheat->Disable();
                cheat->OnDisable();
            }
            else 
            {
                cheat->Enable();
                cheat->OnEnable();
            }
        }
    }

    void Update() {
        for (auto& cheat : cheats_) {
            if (cheat->IsEnabled()) {
                cheat->Update();
            }
        }
    }

    static void RenderAll();

    void Render() {
        for (auto& cheat : cheats_) {
            if (cheat->IsEnabled()) {
                cheat->Draw();
            }
        }
    }

    std::vector<std::string> GetCheatList() const {
        std::vector<std::string> list;
        for (const auto& cheat : cheats_) {
            list.push_back(cheat->GetName() + (cheat->IsEnabled() ? " [ON]" : " [OFF]"));
        }
        return list;
    }

    void Clear() {
        cheats_.clear();
    }

    static CheatManager* GetInstance();

private:
    Cheat* FindCheat(const std::string& name) {
        for (auto& cheat : cheats_) {
            if (cheat->GetName() == name) {
                return cheat.get();
            }
        }
        return nullptr;
    }

    std::vector<std::unique_ptr<Cheat>> cheats_;
};

extern CheatManager* pCheatManager;