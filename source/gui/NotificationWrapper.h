#pragma once

#include <vector>
#include <string>

class Notification {
public:
    std::string title;
    std::string message;
    float displayTime;
    float alpha;
    float timer;
    ImColor textColor;
    ImColor backgroundColor;
    float padding;

    Notification(const std::string& t, const std::string& msg, float time, float pad = 10.0f, ImColor textCol = ImColor(1.0f, 1.0f, 1.0f), ImColor bgCol = ImColor(0.1f, 0.1f, 0.1f))
        : title(t), message(msg), displayTime(time), alpha(1.0f), timer(0.0f), textColor(textCol), backgroundColor(bgCol), padding(pad) {}

    ImVec2 getSize() const {
        ImVec2 titleSize = ImGui::CalcTextSize(title.c_str());
        ImVec2 messageSize = ImGui::CalcTextSize(message.c_str());
        float width = max(titleSize.x, messageSize.x) + 2 * padding;
        float height = titleSize.y + messageSize.y + 3 * padding;
        return ImVec2(width, height);
    }
};

class NotificationManager {
private:
    std::vector<Notification> notifications;

public:
    void addNotification(const std::string& title, const std::string& message, float displayTime, float padding = 10.0f, ImColor textColor = ImColor(1.0f, 1.0f, 1.0f), ImColor bgColor = ImColor(0.1f, 0.1f, 0.1f)) {
        notifications.emplace_back(title, message, displayTime, padding, textColor, bgColor);
    }

    void render()
    {
        auto drawList = ImGui::GetBackgroundDrawList();
        ImVec2 screenSize = ImGui::GetIO().DisplaySize;

        for (auto it = notifications.begin(); it != notifications.end(); )
        {
            it->timer += ImGui::GetIO().DeltaTime;

            if (it->timer >= it->displayTime) {
                it->alpha -= ImGui::GetIO().DeltaTime / 1.0f;
                if (it->alpha <= 0.0f) {
                    it = notifications.erase(it);
                    continue;
                }
            }

            ImVec2 size = it->getSize();
            size.x *= 1.625f;
            //size.y *= 1.35f;

            ImVec2 position(screenSize.x - size.x - 10, screenSize.y - size.y - 10 - (notifications.end() - it - 1) * (size.y + 5));

            drawList->AddRectFilled(position, ImVec2(position.x + size.x, position.y + size.y), ImColor(it->backgroundColor.Value.x, it->backgroundColor.Value.y, it->backgroundColor.Value.z, it->alpha), 10.0f);

            DrawTextWithOutline(drawList,
                ImVec2(position.x + it->padding, position.y + it->padding), it->title.c_str(), ImColor(it->textColor.Value.x, it->textColor.Value.y, it->textColor.Value.z, it->alpha), 1.0f, ImGuiEx::OutlineSide::All, ImColor(0.0f, 0.0f, 0.0f, (float)it->alpha));

            DrawTextWithOutline(drawList,
                ImVec2(position.x + it->padding, position.y + it->padding + ImGui::CalcTextSize(it->title.c_str()).y + 5), it->message.c_str(), ImColor(it->textColor.Value.x, it->textColor.Value.y, it->textColor.Value.z, it->alpha), 1.0f, ImGuiEx::OutlineSide::All, ImColor(0.0f, 0.0f, 0.0f, (float)it->alpha));

            //drawList->AddText(ImVec2(position.x + it->padding, position.y + it->padding), ImColor(it->textColor.Value.x, it->textColor.Value.y, it->textColor.Value.z, it->alpha), it->title.c_str());

            //drawList->AddText(ImVec2(position.x + it->padding, position.y + it->padding + ImGui::CalcTextSize(it->title.c_str()).y + 5), ImColor(it->textColor.Value.x, it->textColor.Value.y, it->textColor.Value.z, it->alpha), it->message.c_str());

            ++it;
        }
    }
};