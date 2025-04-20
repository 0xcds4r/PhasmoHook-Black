#include "../main.h"
#include <array>
#include <unordered_map>
#include <set>
#include "../../library/PhotonHelper.h"

// TODO:
// Transfer features to CheatManager / Update / Render
// Transfer input stuff to ...
// Here only base render and helpers
// Rewrite full bright by fucks shader

NotificationManager notificationManager;

extern II::GameObject* gJackalope;
static II::Vector3 vecBindPos{};
static bool bTeleportMode = false; 
static II::Vector3 teleportTargetPos{}; 
static const float maxTeleportDistance = 120.0f; 

bool GetWorldPositionFromCrosshair(II::Vector3& outPos) 
{
    if (auto* camera = II::Camera::GetMain()) {
        II::Vector2 crosshairPos(ApplicationInfo::screenWidth / 2.0f, ApplicationInfo::screenHeight / 2.0f);

        II::Ray ray = camera->ScreenPointToRay(II::Vector2(crosshairPos.x, crosshairPos.y));
        UnityResolve::UnityType::RaycastHit hitInfo;

        if (UnityResolve::UnityType::Physics::Raycast(ray, &hitInfo, maxTeleportDistance)) {
            outPos = hitInfo.m_Point;
            return true;
        }
    }
    return false;
}

inline void DrawTextFromPosition(std::string_view text, const II::Vector3& position);
void DrawTeleportIndicator(const II::Vector3& position) 
{
    if (auto* camera = II::Camera::GetMain()) 
    {
        float offsetY = 0.1f; 
        float size = 0.5f;

        float height = (sqrt(3.0f) / 2.0f) * size; 

        II::Vector3 v1(position.x - size, position.y + offsetY, position.z - height / 2.0f); 
        II::Vector3 v2(position.x + size, position.y + offsetY, position.z - height / 2.0f);
        II::Vector3 v3(position.x, position.y + offsetY, position.z + height / 2.0f);       

        auto p1 = camera->WorldToScreenPoint(v1, UnityResolve::UnityType::Camera::Eye::Mono);
        auto p2 = camera->WorldToScreenPoint(v2, UnityResolve::UnityType::Camera::Eye::Mono);
        auto p3 = camera->WorldToScreenPoint(v3, UnityResolve::UnityType::Camera::Eye::Mono);

        if (p1.z > 0 && p2.z > 0 && p3.z > 0) {
            p1.y = ApplicationInfo::screenHeight - p1.y;
            p2.y = ApplicationInfo::screenHeight - p2.y;
            p3.y = ApplicationInfo::screenHeight - p3.y;

            ImDrawList* drawList = ImGui::GetBackgroundDrawList();
            drawList->AddTriangleFilled(
                ImVec2(p1.x, p1.y),
                ImVec2(p2.x, p2.y),
                ImVec2(p3.x, p3.y),
                IM_COL32(0, 255, 0, 128) 
            );

            drawList->AddTriangle(
                ImVec2(p1.x, p1.y),
                ImVec2(p2.x, p2.y),
                ImVec2(p3.x, p3.y),
                IM_COL32(255, 255, 255, 255), 
                1.0f
            );

            II::Vector3 labelPos(position.x, position.y + 0.5f, position.z);
            DrawTextFromPosition("Teleport Target", labelPos);
        }
    }
}

ImVec4 HexToRGBA(const std::string& hex) {
    int r, g, b, a;
    sscanf_s(hex.c_str(), "%02x%02x%02x%02x", &r, &g, &b, &a);
    return ImVec4(static_cast<float>(r) / 255, static_cast<float>(g) / 255, static_cast<float>(b) / 255, static_cast<float>(a) / 255);
}

auto GetGhostCurrentRoom() -> LevelRoom*;

struct LightSettings {
    float brightness = 0.0f;
    float range = 0.0f;
};

std::unordered_map<std::string, LightSettings> defaultLightSettings = {
    {"Light", {}},
    {"PCAreaLight", {}},
    {"DirectionalLight", {}}
};

void SetSpecificLightBrightness(const std::string& lightName, float brightness, float range) {
    auto lights = UnityResolve::UnityType::Light::FindAll();
    for (auto* light : lights) {
        if (light && light->GetGameObject()) {
            std::string name = light->GetGameObject()->GetName()->ToString();
            if (name.find(lightName) != std::string::npos) {
                light->SetIntensity(brightness);
                light->SetRange(range);
                return;
            }
        }
    }
}

bool GetSpecificLightBrightness(const std::string& lightName, float* brightness, float* range) {
    auto lights = UnityResolve::UnityType::Light::FindAll();
    for (auto* light : lights) {
        if (light && light->GetGameObject()) {
            std::string name = light->GetGameObject()->GetName()->ToString();
            if (name.find(lightName) != std::string::npos) {
                *brightness = light->GetIntensity();
                *range = light->GetRange();
                return true;
            }
        }
    }
    return false;
}

void StoreDefaultLightSettings() {
    for (auto& [name, settings] : defaultLightSettings) {
        if (settings.brightness == 0.0f && settings.range == 0.0f) {
            GetSpecificLightBrightness(name, &settings.brightness, &settings.range);
        }
    }
}

void ToggleFullBright(bool enable) 
{
    StoreDefaultLightSettings();

    if (enable) {
        SetSpecificLightBrightness("Light", 255.0f, 50.0f);
        SetSpecificLightBrightness("PCAreaLight", 255.0f, 50.0f);
        SetSpecificLightBrightness("Directional Light", 0.1f, 0.1f);
    }
    else {
        for (const auto& [name, settings] : defaultLightSettings) {
            SetSpecificLightBrightness(name, settings.brightness, settings.range);
        }
    }
}

inline void DrawTextFromPosition(std::string_view text, const ImVec2& position) {
    ImGui::GetBackgroundDrawList()->AddText(
        position,
        IM_COL32(255, 255, 255, 255), // Белый цвет по умолчанию
        text.data()
    );
}

inline void DrawTextFromPosition(std::string_view text, const II::Vector3& position) 
{
    if (auto* camera = II::Camera::GetMain()) 
    {
        if (!position.isZero()) {
            auto point = camera->WorldToScreenPoint(position, UnityResolve::UnityType::Camera::Eye::Mono);
            if (point.z > 0) {
                point.y = ApplicationInfo::screenHeight - point.y;
                if (point.x >= 0 && point.x < ApplicationInfo::screenWidth &&
                    point.y >= 0 && point.y < ApplicationInfo::screenHeight) {
                    ImVec2 textSize = ImGui::CalcTextSize(text.data());
                    ImGui::GetBackgroundDrawList()->AddText(
                        { point.x - textSize.x * 0.5f, point.y - textSize.y * 0.5f },
                        IM_COL32(255, 255, 255, 255),
                        text.data()
                    );
                }
            }
        }
    }
}

void ManageEMFList() {
    if (!GhostAI::instance || !ApplicationInfo::bCheatEnabled[CHEAT_EMF_DATA]) return;

    auto now = std::chrono::steady_clock::now();

    for (auto it = Ghost::emfData.begin(); it != Ghost::emfData.end();) {
        auto& [emf, timestamp] = *it;
        if (!emf || !emf->GetGameObject()) {
            it = Ghost::emfData.erase(it);
            continue;
        }

        auto seconds = std::chrono::duration_cast<std::chrono::seconds>(now - timestamp).count();
        FOR_EACH_COMPONENT(emf, II::Transform, "UnityEngine.CoreModule.dll", "Transform", pTransform) {
            if (pTransform) 
            {
                const char* ghostActionTypeStr = "";
                size_t typeIndex = static_cast<size_t>(emf->ghostActionType);
                if (typeIndex < GhostActionTypeNames.size()) {
                    ghostActionTypeStr = GhostActionTypeNames[typeIndex].second.data();
                }

                const char* ghostActionEvStr = "";
                size_t evIndex = static_cast<size_t>(emf->ghostAction);
                if (evIndex < GhostActionEvNames.size()) {
                    ghostActionEvStr = GhostActionEvNames[evIndex].second.data();
                }

                /*if (!Ghost::IsHunting() && !Ghost::IsGhostEvent()) 
                {
                    if (emf->GetGameObject() && emf->ghostActionType == EMFGhostActionType::interaction && emf->ghostAction == EMFGhostActionEv::GhostThrowing)
                    {
                        if (auto instance = Network::GetInstance()) {
                            auto localPlayer = instance->GetLocalPlayer();
                            if (localPlayer) {
                                auto playerTranform = localPlayer->GetTransform();
                                if (playerTranform)
                                {
                                    II::Vector3 vecPos = playerTranform->GetPosition();
                                    if (pTransform->GetPosition().Distance(vecPos) > 5.0f) {
                                        vecPos.y += 0.45f;
                                        emf->GetGameObject()->GetTransform()->SetPosition(vecPos);
                                    }
                                }
                            }
                        }
                    }
                }*/

                std::string strRender = std::format("{} ({})", ghostActionTypeStr, 20 - seconds);
                DrawTextFromPosition(strRender, pTransform->GetPosition());
            }
        }

        if (seconds >= 20) {
            it = Ghost::emfData.erase(it);
        }
        else {
            ++it;
        }
    }
}


/* if (emf->ghostAction == EMFGhostActionEv::GhostThrowing) {
     if (auto instance = Network::GetInstance()) {
         auto localPlayer = instance->GetLocalPlayer();
         if (localPlayer) {
             auto playerTranform = localPlayer->GetTransform();
             if (playerTranform)
             {
                 II::Vector3 vecPos = playerTranform->GetPosition();
                 if (pTransform->GetPosition().Distance(vecPos) > 5.0f) {
                     vecPos.y += 1.5f;
                     pTransform->SetPosition(vecPos);
                 }
             }
         }

     }
 }   */

void ManageBoneEvidence() 
{
    if (!ApplicationInfo::bCheatEnabled[CHEAT_BONE_SHOW]) {
        return;
    }

    if (!GhostAI::instance) {
        return;
    }

    // working at all (SP, MP)
    auto bone = UnityResolve::UnityType::GameObject::Find("Bone");
    if (bone)
    {
        if (auto* transform = bone->GetTransform()) {
            try {
                DrawTextFromPosition("Bone", transform->GetPosition());
            }
            catch (const std::exception& e) {}
            catch (...) {}
        }
    }
}

void ManageEaster()
{
    if (!GhostAI::instance || !gJackalope) {
        return;
    }

    // working at all (SP, MP)
    if (gJackalope)
    {
        if (auto* transform = gJackalope->GetTransform()) {
            try {
                DrawTextFromPosition("jackalope", transform->GetPosition());
            }
            catch (const std::exception& e) {}
            catch (...) {}
        }
    }
}

void ManageCursedItems()
{
    if (!ApplicationInfo::bCheatEnabled[CHEAT_CURSED_ITEMS_SHOW]) {
        return;
    }

    if (!CursedItemsController::instance || !GhostAI::instance) {
        return;
    }

    auto renderItem = [&](auto item, const char* name) {
        try {
            if (item && item->GetGameObject() && item->GetGameObject()->GetTransform()) {
                DrawTextFromPosition(name, item->GetGameObject()->GetTransform()->GetPosition());
            }
        }
        catch (const std::exception& e) {}
        catch (...) {}
    };

    renderItem(CursedItemsController::instance->ouijaBoard, "Ouija Board");
    renderItem(CursedItemsController::instance->musicBox, "Music Box");
    renderItem(CursedItemsController::instance->tarotCards, "Tarot Cards");
    renderItem(CursedItemsController::instance->summoningCircle, "Summoning Circle");
    renderItem(CursedItemsController::instance->hauntedMirror, "Haunted Mirror");
    renderItem(CursedItemsController::instance->voodooDoll, "Voodoo Doll");
    renderItem(CursedItemsController::instance->monkeyPaw, "Monkey Paw");
}

void Gui::ApplyStyles() {
    ImGuiStyle& style = ImGui::GetStyle();

    style.WindowPadding = ImVec2(15.0f, 15.0f);
    style.FramePadding = ImVec2(5.0f, 4.0f);
    style.ItemSpacing = ImVec2(12.0f, 8.0f);
    style.ItemInnerSpacing = ImVec2(8.0f, 6.0f);
    style.IndentSpacing = 25.0f;
    style.ScrollbarSize = 15.0f;
    style.GrabMinSize = 15.0f;
    style.WindowRounding = 15.0f;
    style.ScrollbarRounding = 15.0f;
    style.GrabRounding = 7.0f;
    style.ChildRounding = 8.0f;
    style.FrameRounding = 6.0f;
    style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
    style.AntiAliasedFill = style.AntiAliasedLines = style.AntiAliasedLinesUseTex = true;

    ImVec4* colors = style.Colors;
    colors[ImGuiCol_Text] = ImVec4(0.95f, 0.96f, 0.98f, 1.0f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.36f, 0.42f, 0.47f, 1.0f);
    colors[ImGuiCol_WindowBg] = ImVec4(0.11f, 0.15f, 0.17f, 1.0f);
    colors[ImGuiCol_ChildBg] = ImVec4(0.15f, 0.18f, 0.22f, 1.0f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
    colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.25f, 0.29f, 1.0f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.12f, 0.20f, 0.28f, 1.0f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.09f, 0.12f, 0.14f, 1.0f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.09f, 0.12f, 0.14f, 0.65f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.08f, 0.10f, 0.12f, 1.0f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.20f, 0.25f, 0.29f, 1.0f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.18f, 0.22f, 0.25f, 1.0f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.09f, 0.21f, 0.31f, 1.0f);
    colors[ImGuiCol_CheckMark] = ImVec4(0.28f, 0.56f, 1.0f, 1.0f);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.28f, 0.56f, 1.0f, 1.0f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.37f, 0.61f, 1.0f, 1.0f);
    colors[ImGuiCol_Button] = ImVec4(0.20f, 0.25f, 0.29f, 1.0f);
    colors[ImGuiCol_ButtonHovered] = COLOR_RED;
    colors[ImGuiCol_ButtonActive] = COLOR_RED;
}

extern auto ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM) -> LRESULT;
void UpdateCursorClip()
{
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    RECT centerRect;
    centerRect.left = screenWidth / 2 - 1;
    centerRect.top = screenHeight / 2 - 1;
    centerRect.right = screenWidth / 2 + 1;
    centerRect.bottom = screenHeight / 2 + 1;

    ClipCursor(&centerRect);
    ClipCursor(nullptr);
}

char Gui::ProcessInput(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) 
{
    HotKey::PotMsg(msg);

    static const std::unordered_map<WPARAM, std::pair<int, const char*>> hotkeys = {
        {0x31, {CHEAT_PLAYER_ANTISTAMINA, "Disable Stamina"}},
        {0x32, {CHEAT_GHOST_WALLHACK, "Ghost Wallhack"}},
        {0x33, {CHEAT_HUNT_NOTIFIES, "Hunt notifications"}},
        {0x34, {CHEAT_GHOST_INFO_SHOW, "Ghost info show"}},
        {0x35, {CHEAT_EMF_DATA, "EMF Show"}},
        {0x36, {CHEAT_FULLBRIGHT, "FullBright"}},
        {0x37, {CHEAT_CURSED_ITEMS_SHOW, "Cursed Items Show"}},
        {0x38, {CHEAT_BONE_SHOW, "Bone Show"}}
    };

    switch (msg) 
    {
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN: 
        {
            bool isRightShift = wParam == VK_SHIFT && lParam == 0x360001;

            if (isRightShift) 
            {
                ApplicationInfo::bMenuActive = !ApplicationInfo::bMenuActive;
                ShowCursor(ApplicationInfo::bMenuActive);
                UpdateCursorClip();
                return 1;
            }
            else if (wParam == 0x30) // key 0
            {
                if (ApplicationInfo::bMenuActive) {
                    break;
                }

                if (gJackalope && gJackalope->GetTransform()) {
                    AddNotify("Goto Jackalope",
                        "Success",
                        3.5f, COLOR_WHITE, COLOR_BLUE_HOVER);

                    auto player = UnityResolve::UnityType::GameObject::Find("PCPlayer(Clone)");
                    if (player) {
                        if (player->GetTransform()) {
                            II::Vector3 jaPos = gJackalope->GetTransform()->GetPosition();
                            jaPos.y += 0.25f;
                            player->GetTransform()->SetPosition(jaPos);
                        }
                    }
                }
                else {
                    AddNotify("Goto Jackalope",
                        "Jackalope not found!",
                        3.5f, COLOR_WHITE, COLOR_RED_HOVER);
                }

                return 1;
            }
            else if (wParam == VK_PRIOR) // Page Up
            {
                if (ApplicationInfo::bMenuActive) {
                    break;
                }

                auto player = UnityResolve::UnityType::GameObject::Find("PCPlayer(Clone)");
                if (player) {
                    vecBindPos = { player->GetTransform()->GetPosition().x, player->GetTransform()->GetPosition().y, player->GetTransform()->GetPosition().z };
                    AddNotify("Pos binded",
                        "(XYZ: " + std::to_string(vecBindPos.x) + ", " + std::to_string(vecBindPos.y) + ", " + std::to_string(vecBindPos.z) + ")",
                        3.5f, COLOR_WHITE, COLOR_BLUE_HOVER);
                }
               
                return 1;
            }
            else if (wParam == VK_NEXT) // Page Down
            {
                if (ApplicationInfo::bMenuActive) {
                    break;
                }

                if (vecBindPos.isZero()) {
                    AddNotify("Goto binded pos",
                        "Failed: not set",
                        3.5f, COLOR_WHITE, COLOR_RED_HOVER);
                    break;
                }

                auto player = UnityResolve::UnityType::GameObject::Find("PCPlayer(Clone)");
                if (player) {
                    if (player->GetTransform()) {
                        AddNotify("Goto binded pos",
                            "Success",
                            3.5f, COLOR_WHITE, COLOR_BLUE_HOVER);
                        player->GetTransform()->SetPosition(vecBindPos);
                    }
                }
                return 1;
            }

            else if (auto it = hotkeys.find(wParam); it != hotkeys.end()) 
            {
                if (ApplicationInfo::bMenuActive) {
                    break;
                }

                const auto& [cheatId, title] = it->second;
                ApplicationInfo::bCheatEnabled[cheatId] = !ApplicationInfo::bCheatEnabled[cheatId];

                if (ApplicationInfo::IsEngLocale()) {
                    AddNotify(title,
                        ApplicationInfo::bCheatEnabled[cheatId] ? "Toggle: Enabled" : "Toggle: Disabled",
                        3.5f, COLOR_WHITE, COLOR_BLUE_HOVER);
                }
                else {
                    AddNotify(title,
                        ApplicationInfo::bCheatEnabled[cheatId] ? "Состояние: Включено" : "Состояние: Отключено",
                        3.5f, COLOR_WHITE, COLOR_BLUE_HOVER);
                }

                if(cheatId == CHEAT_FULLBRIGHT)
                    ToggleFullBright(ApplicationInfo::bCheatEnabled[CHEAT_FULLBRIGHT]);
               
                return 1;
            }
            break;
        }

        case WM_MBUTTONDOWN:
        {
            if (ApplicationInfo::bMenuActive) {
                break;
            }

            if (!Game::isOnMission) {
                break;
            }

            bTeleportMode = !bTeleportMode;
            if (bTeleportMode) {
                if (GetWorldPositionFromCrosshair(teleportTargetPos)) {
                    AddNotify("Teleport Mode",
                        "Enabled: Click LMB to teleport, MMB to cancel",
                        3.5f, COLOR_WHITE, COLOR_BLUE_HOVER);
                }
                else {
                    AddNotify("Teleport Mode",
                        "Enabled: No valid target found yet",
                        3.5f, COLOR_WHITE, COLOR_BLUE_HOVER);
                }
            }
            else {
                AddNotify("Teleport Mode",
                    "Disabled",
                    3.5f, COLOR_WHITE, COLOR_BLUE_HOVER);
            }
            return 1;
        }

        case WM_LBUTTONDOWN: 
        {
            if (ApplicationInfo::bMenuActive || !bTeleportMode) {
                break;
            }

            if (!Game::isOnMission) {
                break;
            }

            auto player = UnityResolve::UnityType::GameObject::Find("PCPlayer(Clone)");
            if (player && player->GetTransform()) {
                teleportTargetPos.y += 0.65f; 
                player->GetTransform()->SetPosition(teleportTargetPos);
                AddNotify("Teleport",
                    "Success: Teleported to target",
                    3.5f, COLOR_WHITE, COLOR_BLUE_HOVER);
            }
            else {
                AddNotify("Teleport",
                    "Failed: Player not found",
                    3.5f, COLOR_WHITE, COLOR_RED_HOVER);
            }

            bTeleportMode = false;
            return 1;
        }

        case WM_CLOSE:
            std::exit(0);
            break;
    }
    
    return ApplicationInfo::bMenuActive ? 0 : 1;
}

void Gui::AddNotify(const std::string& title, const std::string& message, float displayTime, ImColor textColor, ImColor bgColor) {
    notificationManager.addNotification(title, message, displayTime, 10.0f, textColor, bgColor);
}

void Gui::updateLoop(std::chrono::steady_clock::time_point time) {
    lastFrameTime = time;
}

void Gui::ManageHuntNotifies() {
    if (ApplicationInfo::bCheatEnabled[CHEAT_HUNT_NOTIFIES])
    {
        static bool bHunting = false;
        bool isHunting = Ghost::IsHunting();

        if (isHunting != bHunting) 
        {
            if (ApplicationInfo::IsEngLocale()) {
                if (isHunting) {
                    AddNotify("[!] HUNTING START", "The ghost has started the hunt",
                        3.5f, COLOR_WHITE, COLOR_RED);
                }
                else {
                    AddNotify("[!] HUNTING END", "The ghost stopped the hunt",
                        3.5f, COLOR_WHITE, COLOR_BLUE_HOVER);
                }
            }
            else 
            {
                if (isHunting) {
                    AddNotify("[!] НАЧАЛАСЬ ОХОТА", "Призрак начал охоту",
                        3.5f, COLOR_WHITE, COLOR_RED);
                }
                else {
                    AddNotify("[!] ОХОТА ЗАКОНЧИЛАСЬ", "Призрак закончил охоту",
                        3.5f, COLOR_WHITE, COLOR_BLUE_HOVER);
                }
            }
           
            bHunting = isHunting;
        }
    }
}

void Gui::ManagePlayersWallhack()
{
    if (!ApplicationInfo::bCheatEnabled[CHEAT_PLAYERS_WALLHACK]) {
        return;
    }

    if (!II::Camera::GetMain()) {
        return;
    }

    auto players = Players::GetAllPlayers();
    if (players.empty()) return;

    for (auto player : players) {
        player->Draw();
    }

    //if (ApplicationInfo::bCheatEnabled[CHEAT_PLAYERS_WALLHACK] && II::Camera::GetMain())
    //{
        //Log("Gui::ManagePlayersWallhack -> Starting wallhack processing");
    //    auto players = Players::GetAllPlayers(); 
    //    if (!players.empty())
    //    {
    //        std::vector<std::pair<int, II::Animator*>> entities;
    //        entities.clear();

    //        //Log("Gui::ManagePlayersWallhack -> Found " + std::to_string(players.size()) + " players");
    //        for (auto player : players)
    //        {
    //            if (!player) {
    //                //Log("Gui::ManagePlayersWallhack -> Warning: Null player encountered");
    //                continue;
    //            }

    //            auto gamePlayer = player->GetPlayer();
    //            if (gamePlayer && player->GetName())
    //            {
    //                //Log("Gui::ManagePlayersWallhack -> Processing player: " + std::string(player->GetName()));
    //                if (gamePlayer->GetGameObject())
    //                {
    //                    auto pTransform = gamePlayer->GetTransform();
    //                    if (pTransform)
    //                    {
    //                        try {
    //                            II::Vector3 position = pTransform->GetPosition();
    //                            II::Vector3 up = pTransform->GetUp();

    //                            float heightOffset = 0.95f;
    //                            std::string namePlusID = std::format("({}) {}", player->GetID(), player->GetName());
    //                            II::Vector3 headPosition = position + (up * heightOffset);

    //                            auto camera = II::Camera::GetMain();
    //                            auto screenPos = camera->WorldToScreenPoint(headPosition, UnityResolve::UnityType::Camera::Eye::Mono);

    //                            if (screenPos.z > 0) { 
    //                                screenPos.y = ApplicationInfo::screenHeight - screenPos.y;

    //                                ImVec2 textSize = ImGui::CalcTextSize(namePlusID.c_str());

    //                                ImVec2 textPos = ImVec2(
    //                                    screenPos.x - textSize.x * 0.5f,
    //                                    screenPos.y - textSize.y - 5.0f  
    //                                );

    //                                DrawTextFromPosition(namePlusID, textPos);

    //                                FOR_EACH_COMPONENT(gamePlayer, II::Animator, "UnityEngine.AnimationModule.dll", "Animator", pAnimator) {
    //                                    if (pAnimator) {
    //                                        entities.emplace_back(player->GetID(), pAnimator);
    //                                    }
    //                                };
    //                            }
    //                        }
    //                        catch (const std::exception& e) {
    //                            //Log("Gui::ManagePlayersWallhack -> Exception while drawing for " +
    //                                //std::string(player->GetName()) + ": " + e.what());
    //                        }
    //                        catch (...) {
    //                            //Log("Gui::ManagePlayersWallhack -> Unknown exception while drawing for " +
    //                                //std::string(player->GetName()));
    //                        }
    //                    }
    //                    else {
    //                        //Log("Gui::ManagePlayersWallhack -> Failed to get transform for " +
    //                            //std::string(player->GetName()));
    //                    }
    //                }
    //                else {
    //                    //Log("Gui::ManagePlayersWallhack -> Failed to get game object for " +
    //                        //std::string(player->GetName()));
    //                }
    //            }
    //            else {
    //                //Log("Gui::ManagePlayersWallhack -> Invalid player data: gamePlayer=" +
    //                    //std::to_string(reinterpret_cast<uintptr_t>(gamePlayer)) +
    //                    //", name=" + (player->GetName() ? player->GetName() : "null"));
    //            }

    //            if (!entities.empty()) {
    //                playersSESP.DrawMultipleBoneConnections(entities, 240, 32, 165, 255);
    //            }
    //        }
    //    }
    //    else {
    //        //Log("Gui::ManagePlayersWallhack -> No players found");
    //    }
    //}
    //else {
    //    //Log("Gui::ManagePlayersWallhack -> Wallhack disabled or no main camera");
    //}
}

void ResetAllGUI() {
    //ghostSESP.ClearAllBoneCache();    
}

struct BonePair {
    UnityResolve::UnityType::Animator::HumanBodyBones start;
    UnityResolve::UnityType::Animator::HumanBodyBones end;

    bool operator<(const BonePair& other) const {
        if (start != other.start) {
            return start < other.start;
        }
        return end < other.end;
    }
};

constexpr std::array<BonePair, 20> bonePairs = { {
    {II::Animator::HumanBodyBones::Head, II::Animator::HumanBodyBones::Neck},
    {II::Animator::HumanBodyBones::Neck, II::Animator::HumanBodyBones::UpperChest},
    {II::Animator::HumanBodyBones::UpperChest, II::Animator::HumanBodyBones::Chest},
    {II::Animator::HumanBodyBones::Chest, II::Animator::HumanBodyBones::Spine},
    {II::Animator::HumanBodyBones::Spine, II::Animator::HumanBodyBones::Hips},
    {II::Animator::HumanBodyBones::LeftShoulder, II::Animator::HumanBodyBones::LeftUpperArm},
    {II::Animator::HumanBodyBones::LeftUpperArm, II::Animator::HumanBodyBones::LeftLowerArm},
    {II::Animator::HumanBodyBones::LeftLowerArm, II::Animator::HumanBodyBones::LeftHand},
    {II::Animator::HumanBodyBones::RightShoulder, II::Animator::HumanBodyBones::RightUpperArm},
    {II::Animator::HumanBodyBones::RightUpperArm, II::Animator::HumanBodyBones::RightLowerArm},
    {II::Animator::HumanBodyBones::RightLowerArm, II::Animator::HumanBodyBones::RightHand},
    {II::Animator::HumanBodyBones::LeftUpperLeg, II::Animator::HumanBodyBones::LeftLowerLeg},
    {II::Animator::HumanBodyBones::LeftLowerLeg, II::Animator::HumanBodyBones::LeftFoot},
    {II::Animator::HumanBodyBones::LeftFoot, II::Animator::HumanBodyBones::LeftToes},
    {II::Animator::HumanBodyBones::RightUpperLeg, II::Animator::HumanBodyBones::RightLowerLeg},
    {II::Animator::HumanBodyBones::RightLowerLeg, II::Animator::HumanBodyBones::RightFoot},
    {II::Animator::HumanBodyBones::RightFoot, II::Animator::HumanBodyBones::RightToes},
    {II::Animator::HumanBodyBones::LeftEye, II::Animator::HumanBodyBones::Head},
    {II::Animator::HumanBodyBones::RightEye, II::Animator::HumanBodyBones::Head},
    {II::Animator::HumanBodyBones::Jaw, II::Animator::HumanBodyBones::Head}
} };

bool IsValidScreenPosition(const II::Vector3& point, int screenWidth, int screenHeight) {
    if (!std::isfinite(point.x) || !std::isfinite(point.y) || !std::isfinite(point.z)) {
        return false;
    }

    if (point.z <= 0) {
        return false;
    }

    float screenY = static_cast<float>(screenHeight) - point.y;
    return (point.x >= 0 && point.x <= static_cast<float>(screenWidth) &&
        screenY >= 0 && screenY <= static_cast<float>(screenHeight));
}

void DrawBoneConnectionsEx(II::Animator* pAnimator, int r = 255, int g = 255, int b = 255, int a = 255) {
    if (!pAnimator) {
        return;
    }

    II::Camera* camera = II::Camera::GetMain();
    if (!camera || !camera->GetTransform()) {
        return;
    }

    ImGuiIO& io = ImGui::GetIO();
    if (!io.DisplaySize.x || !io.DisplaySize.y) {
        return;
    }

    auto* drawList = ImGui::GetBackgroundDrawList();
    if (!drawList) {
        return;
    }

    ImU32 color = IM_COL32(r, g, b, a);
    const int screenWidth = static_cast<int>(io.DisplaySize.x);
    const int screenHeight = static_cast<int>(io.DisplaySize.y);

    try {
        for (const auto& pair : bonePairs) {
            auto* startTransform = pAnimator->GetBoneTransform(pair.start);
            auto* endTransform = pAnimator->GetBoneTransform(pair.end);
            if (!startTransform || !endTransform) {
                continue;
            }

            auto startPos = startTransform->GetPosition();
            auto endPos = endTransform->GetPosition();

            if (!std::isfinite(startPos.x) || !std::isfinite(startPos.y) || !std::isfinite(startPos.z) ||
                !std::isfinite(endPos.x) || !std::isfinite(endPos.y) || !std::isfinite(endPos.z)) {
                continue;
            }

            auto startPoint = camera->WorldToScreenPoint(startPos, UnityResolve::UnityType::Camera::Eye::Mono);
            auto endPoint = camera->WorldToScreenPoint(endPos, UnityResolve::UnityType::Camera::Eye::Mono);

            if (IsValidScreenPosition(startPoint, screenWidth, screenHeight) &&
                IsValidScreenPosition(endPoint, screenWidth, screenHeight)) {

                drawList->AddLine(
                    ImVec2(startPoint.x, screenHeight - startPoint.y),
                    ImVec2(endPoint.x, screenHeight - endPoint.y),
                    color,
                    1.5f
                );
            }
        }
    }
    catch (const std::exception& e) {
        printf("Error in DrawBoneConnectionsNoCache: %s\n", e.what());
    }
}

void Gui::ManageGhostWallhack() 
{
    if (!GhostAI::instance) return;

    if (ApplicationInfo::bCheatEnabled[CHEAT_GHOST_WALLHACK]) 
    {
        FOR_EACH_COMPONENT(GhostAI::instance, II::Animator, "UnityEngine.AnimationModule.dll", "Animator", pAnimator) 
        {
            if (pAnimator) 
            {
                bool bGhostEvent = Ghost::IsGhostEvent();
                bool bHunt = Ghost::IsHunting();
                bool bIncense = Ghost::IncenseEffect();
                bool bUsingAbility = Ghost::GetState(GhostAI::instance) == GhostAI::States::GhostAbility;

                if (bGhostEvent) {
                    DrawBoneConnectionsEx(pAnimator, 0, 255, 0, 255);
                }
                else if (bHunt) {
                    DrawBoneConnectionsEx(pAnimator, 255, 0, 0, 255);
                }
                else if (bIncense) {
                    DrawBoneConnectionsEx(pAnimator, 0, 0, 255, 255);
                }
                else if(bUsingAbility) {
                    DrawBoneConnectionsEx(pAnimator, 255, 255, 0, 255);
                }
                else {
                    DrawBoneConnectionsEx(pAnimator, 255, 255, 255, 255);
                }
            }
        }
    }
}

void Gui::DisplayGhostInfo()
{
    static int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    static int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    if (ApplicationInfo::bCheatEnabled[CHEAT_GHOST_INFO_SHOW])
    {
        ImDrawList* drawList = ImGui::GetBackgroundDrawList();

        ImVec2 screenSize = ImVec2(screenWidth, screenHeight);
        float baseY = screenSize.y - 10 - ImGui::GetTextLineHeight();

        auto DrawOutlinedText = [&](const char* label, const char* text, ImVec2 position,
            ImU32 outlineColor = ImColor(0, 0, 0, 255),
            ImU32 textColor = ImColor(255, 255, 255, 255)) {
                ImVec2 labelSize = ImGui::CalcTextSize(cp1251_to_utf8(label).c_str());
                ImVec2 textSize = ImGui::CalcTextSize(cp1251_to_utf8(text).c_str());

                for (int x = -1; x <= 1; x++) {
                    for (int y = -1; y <= 1; y++) {
                        if (x != 0 || y != 0) {
                            drawList->AddText(ImVec2(position.x + x, position.y + y), outlineColor, cp1251_to_utf8(label).c_str());
                            drawList->AddText(ImVec2(position.x + labelSize.x + x, position.y + y), outlineColor, cp1251_to_utf8(text).c_str());
                        }
                    }
                }
                drawList->AddText(position, textColor, cp1251_to_utf8(label).c_str());
                drawList->AddText(ImVec2(position.x + labelSize.x, position.y), textColor, cp1251_to_utf8(text).c_str());
            };

        if (Ghost::GetInfo(GhostAI::instance))
        {
            LevelRoom* pRoom = GetGhostCurrentRoom();
            if (pRoom)
            {
                float xOffset = 10.0f;
                const float minSpacing = 20.0f;

                if (ApplicationInfo::IsEngLocale()) {
                    DrawOutlinedText("Type: ", Ghost::GetTypeName(Ghost::GetType()), ImVec2(xOffset, baseY));
                    xOffset += ImGui::CalcTextSize("Type: ").x + ImGui::CalcTextSize(Ghost::GetTypeName(Ghost::GetType())).x + minSpacing;

                    DrawOutlinedText("Name: ", Ghost::GetName(), ImVec2(xOffset, baseY));
                    xOffset += ImGui::CalcTextSize("Name: ").x + ImGui::CalcTextSize(Ghost::GetName()).x + minSpacing;

                    bool bGhostEvent = Ghost::IsGhostEvent();
                    ImU32 eventColor = bGhostEvent ? ImColor(0, 255, 0, 255) : ImColor(255, 255, 255, 255);
                    DrawOutlinedText("Event: ", bGhostEvent ? "Yes" : "No", ImVec2(xOffset, baseY),
                        ImColor(0, 0, 0, 255), eventColor);
                    xOffset += ImGui::CalcTextSize("Event: ").x + ImGui::CalcTextSize(bGhostEvent ? "Yes" : "No").x + minSpacing;

                    bool bIncense = Ghost::IncenseEffect();
                    ImU32 incenseColor = bIncense ? ImColor(0, 255, 0, 255) : ImColor(255, 255, 255, 255);
                    DrawOutlinedText("Incense: ", bIncense ? "Yes" : "No", ImVec2(xOffset, baseY),
                        ImColor(0, 0, 0, 255), incenseColor);
                    xOffset += ImGui::CalcTextSize("Incense: ").x + ImGui::CalcTextSize(bIncense ? "Yes" : "No").x + minSpacing;

                    if (Ghost::GetType() == GhostTraits::GhostType::Banshee) {
                        DrawOutlinedText("Target: ", Ghost::GetBansheeTargetNickname(), ImVec2(xOffset, baseY));
                        xOffset += ImGui::CalcTextSize("Target: ").x + ImGui::CalcTextSize(Ghost::GetBansheeTargetNickname()).x + minSpacing;
                    }

                    if (Ghost::GetType() == GhostTraits::GhostType::Mimic) {
                        DrawOutlinedText("Imit: ", Ghost::GetTypeName(Ghost::GetMimicType()), ImVec2(xOffset, baseY));
                        xOffset += ImGui::CalcTextSize("Imit: ").x + ImGui::CalcTextSize(Ghost::GetTypeName(Ghost::GetMimicType())).x + minSpacing;
                    }

                    bool isHunting = Ghost::IsHunting();
                    ImU32 stateColor = isHunting ? ImColor(255, 0, 0, 255) : ImColor(255, 255, 255, 255);
                    DrawOutlinedText("State: ", isHunting ? "Hunting" : Ghost::GetStateName(), ImVec2(xOffset, baseY),
                        ImColor(0, 0, 0, 255), stateColor);
                    xOffset += ImGui::CalcTextSize("State: ").x + ImGui::CalcTextSize(isHunting ? "Hunting" : Ghost::GetStateName()).x + minSpacing;

                    DrawOutlinedText("Room: ", Room::GetRoomName(pRoom), ImVec2(xOffset, baseY));
                    xOffset += ImGui::CalcTextSize("Room: ").x + ImGui::CalcTextSize(Room::GetRoomName(pRoom)).x + minSpacing;

                    std::string tempStr = std::format("{:.1f}", Room::GetRoomTemperature(pRoom));
                    DrawOutlinedText("Temp: ", tempStr.c_str(), ImVec2(xOffset, baseY));
                }
                else {
                    DrawOutlinedText("Тип: ", Ghost::GetTypeName(Ghost::GetType(), true), ImVec2(xOffset, baseY));
                    xOffset += ImGui::CalcTextSize(cp1251_to_utf8("Тип: ").c_str()).x + ImGui::CalcTextSize(Ghost::GetTypeName(Ghost::GetType(), true)).x + minSpacing;

                    DrawOutlinedText("Имя: ", Ghost::GetName(), ImVec2(xOffset, baseY));
                    xOffset += ImGui::CalcTextSize(cp1251_to_utf8("Имя: ").c_str()).x + ImGui::CalcTextSize(Ghost::GetName()).x + minSpacing;

                    bool bGhostEvent = Ghost::IsGhostEvent();
                    ImU32 eventColor = bGhostEvent ? ImColor(0, 255, 0, 255) : ImColor(255, 255, 255, 255);

                    DrawOutlinedText("Событие: ", bGhostEvent ? "Да" : "Нет", ImVec2(xOffset, baseY),
                        ImColor(0, 0, 0, 255), eventColor);

                    xOffset += ImGui::CalcTextSize(cp1251_to_utf8("Событие: ").c_str()).x + ImGui::CalcTextSize(bGhostEvent ? cp1251_to_utf8("Да").c_str() : cp1251_to_utf8("Нет").c_str()).x + minSpacing;

                    bool bIncense = Ghost::IncenseEffect();
                    ImU32 incenseColor = bIncense ? ImColor(0, 255, 0, 255) : ImColor(255, 255, 255, 255);
                    DrawOutlinedText("Благовоние: ", bIncense ? "Да" : "Нет", ImVec2(xOffset, baseY),
                        ImColor(0, 0, 0, 255), incenseColor);
                    xOffset += ImGui::CalcTextSize(cp1251_to_utf8("Благовоние: ").c_str()).x + ImGui::CalcTextSize(bIncense ? cp1251_to_utf8("Да").c_str() : cp1251_to_utf8("Нет").c_str()).x + minSpacing;

                    if (Ghost::GetType() == GhostTraits::GhostType::Banshee && Ghost::GetBansheeTargetNickname()) {
                        DrawOutlinedText("Цель: ", Ghost::GetBansheeTargetNickname(), ImVec2(xOffset, baseY));
                        xOffset += ImGui::CalcTextSize(cp1251_to_utf8("Цель: ").c_str()).x + ImGui::CalcTextSize(cp1251_to_utf8(Ghost::GetBansheeTargetNickname()).c_str()).x + minSpacing;
                    }

                    if (Ghost::GetType() == GhostTraits::GhostType::Mimic) {
                        DrawOutlinedText("Имитирует: ", Ghost::GetTypeName(Ghost::GetMimicType(), true), ImVec2(xOffset, baseY));
                        xOffset += ImGui::CalcTextSize("Имитирует: ").x + ImGui::CalcTextSize(Ghost::GetTypeName(Ghost::GetMimicType(), true)).x + minSpacing;
                    }

                    bool isHunting = Ghost::IsHunting();
                    ImU32 stateColor = isHunting ? ImColor(255, 0, 0, 255) : ImColor(255, 255, 255, 255);
                    DrawOutlinedText("Состояние: ", isHunting ? "Охота" : Ghost::GetStateName(), ImVec2(xOffset, baseY),
                        ImColor(0, 0, 0, 255), stateColor);
                    xOffset += ImGui::CalcTextSize(cp1251_to_utf8("Состояние: ").c_str()).x + ImGui::CalcTextSize(isHunting ? cp1251_to_utf8("Охота").c_str() : cp1251_to_utf8(Ghost::GetStateName()).c_str()).x + minSpacing;

                    DrawOutlinedText("Комната: ", Room::GetRoomName(pRoom), ImVec2(xOffset, baseY));
                    xOffset += ImGui::CalcTextSize(cp1251_to_utf8("Комната: ").c_str()).x + ImGui::CalcTextSize(Room::GetRoomName(pRoom)).x + minSpacing;

                    std::string tempStr = std::format("{:.1f}", Room::GetRoomTemperature(pRoom));
                    DrawOutlinedText("Температура: ", tempStr.c_str(), ImVec2(xOffset, baseY));
                }
                
                // ghost wiki..
                /*ImVec2 pos(15,15);

                char bufferDesc[8192+1]{};
                sprintf(bufferDesc, " %s", Ghost::GetDescription(ApplicationInfo::IsRusLocale()));
                for (int i = 0; bufferDesc[i] != '\0'; i++) {
                    if (bufferDesc[i] == '.') {
                        bool prevIsDigit = (i > 0 && isdigit(bufferDesc[i - 1]));
                        bool nextIsDigit = (bufferDesc[i + 1] != '\0' && isdigit(bufferDesc[i + 1]));
                        if (!prevIsDigit && !nextIsDigit) {
                            bufferDesc[i] = '\n';
                        }
                    }
                }

                char* lineStart = bufferDesc;
                char* currentPos = bufferDesc;
                while (*currentPos != '\0') {
                    if (*currentPos == '\n') {
                        *currentPos = '\0'; 
                        DrawOutlinedText("", lineStart, pos);
                        pos.y += 30.0f; 
                        *currentPos = '\n';
                        lineStart = currentPos + 1;
                    }
                    currentPos++;
                }
                if (lineStart < currentPos) {
                    DrawOutlinedText("", lineStart, pos);
                }*/
            }
        }
    }
}

void Gui::ManageInjectNotifies() {
    static bool bInjected = false;
    if (!bInjected) {
        AddNotify("Phasmohook", "Successfully injected!\n\nPress 'RSHIFT' key for Open Menu!",
            3.5f, COLOR_WHITE, COLOR_DARK_BLUE);
        bInjected = true;
    }
}

void Gui::UpdateDeltaTime() {
    auto currentFrameTime = std::chrono::high_resolution_clock::now();
    currentDeltaTime = std::chrono::duration<float>(currentFrameTime - lastFrameTime).count();
    lastFrameTime = currentFrameTime;
    ImGui::GetIO().DeltaTime = currentDeltaTime.load();
}
auto UNITY_CALLING_CONVENTION Player__ToggleFreezePlayer(Player* _this, bool state) -> void;
bool noclip_initialized = false;
void Gui::ManageNoclip() {
    
}

void Gui::ResetNoclip() {
    auto player = Players::GetLocalPlayer()->GetPlayer();
    if (!player) {
        //AddNotify("No Clip", "Player not found!", 3.5f, COLOR_WHITE, COLOR_RED);
        return;
    }

    auto gameObject = player->GetGameObject();
    if (!gameObject) {
        return;
    }

    auto rigidbody = gameObject->GetComponent<UnityResolve::UnityType::Rigidbody*>(
        UnityResolve::Get("UnityEngine.PhysicsModule.dll")->Get("Rigidbody")
    );

    auto collider = gameObject->GetComponent<UnityResolve::UnityType::Collider*>(
        UnityResolve::Get("UnityEngine.PhysicsModule.dll")->Get("Collider")
    );

    auto characterController = gameObject->GetComponent<UnityResolve::UnityType::CharacterController*>(
        UnityResolve::Get("UnityEngine.PhysicsModule.dll")->Get("CharacterController")
    );

    auto physicsController = gameObject->GetComponent<PhysicsCharacterController*>(
        UnityResolve::Get("Assembly-CSharp.dll")->Get("PhysicsCharacterController")
    );

    

    if (rigidbody) {
        rigidbody->SetDetectCollisions(true);
        //rigidbody->SetUseGravity(true);
        //rigidbody->SetIsKinematic(false);
        //rigidbody->SetInterpolation(UnityResolve::UnityType::RigidbodyInterpolation::Interpolate); 
    }
    /*if (collider) {
        collider->SetEnabled(true);
    }*/

    /*if (physicsController) {
        auto capsule = physicsController->GetCapsuleCollider();
        if (capsule) {
            capsule->SetRadius(0.35f);
        }
    }*/

    noclip_initialized = false;
    //Player__ToggleFreezePlayer(player, false);
}

void Gui::DoDrawFeatures() 
{
    ManageInjectNotifies();
    
    if (Game::isOnMission && GhostAI::instance)
    {
        ManageEMFList();
        ManageBoneEvidence();
        ManageHuntNotifies();
        ManageGhostWallhack();
        DisplayGhostInfo();
        ManageCursedItems();
        ManageNoclip();
        ManageEaster();

        if (bTeleportMode) {
            if (GetWorldPositionFromCrosshair(teleportTargetPos)) {
                DrawTeleportIndicator(teleportTargetPos);
            }
            else {
                //bTeleportMode = false;
                /*AddNotify("Teleport Mode",
                    "Disabled: No valid target found",
                    3.5f, COLOR_WHITE, COLOR_RED_HOVER);*/
            }
        }
    }

    ManagePlayersWallhack();

    ProcessCheats();
}

// Sidebar rendering
void Gui::RenderSideBar() 
{
    static constexpr float buttonHeight = 45.0f;
    static const std::array<std::pair<const char*, int>, 4> buttons = { {
        {"Player", 0}, {"Ghost", 1}, {"Preferences", 2}, {"Console", 3}
    } };

    ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.5f, 0.5f)); 
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10.0f, 10.0f));

    for (const auto& [label, page] : buttons)
    {
        if (selectedPage == page) {
            ImGui::PushStyleColor(ImGuiCol_Button, COLOR_RED);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, COLOR_RED_HOVER); 
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, COLOR_RED_ACTIVE);
        }

        if (ImGui::Button(label, ImVec2(-1, buttonHeight))) {
            selectedPage = page;
        }

        if (selectedPage == page) {
            ImGui::PopStyleColor(3);
        }

        if (page != buttons.back().second) {
            ImGui::Spacing();
        }
    }

    ImGui::PopStyleVar(2);
}

void Gui::RenderMainContent() {
    static const ImVec4 titleColor(0.7f, 0.9f, 0.7f, 1.0f);

    switch (selectedPage) {
    case 0: // Player
        RenderPlayerPage(titleColor);
        break;

    case 1: // Ghost
        RenderGhostPage(titleColor);
        break;

    case 2: // Preferences
        RenderPreferencesPage(titleColor);
        break;

    case 3: // Console
        RenderConsolePage(titleColor);
        break;
    }
}

void Gui::RenderPlayerPage(const ImVec4& titleColor) {
    ImGui::TextColored(titleColor, "Game Player / Cheats");
    ImGui::Separator();

    if (ImGui::Checkbox("Disable Stamina", &ApplicationInfo::bCheatEnabled[CHEAT_PLAYER_ANTISTAMINA])) {
        AddNotify("Disable Stamina",
            ApplicationInfo::bCheatEnabled[CHEAT_PLAYER_ANTISTAMINA] ? "Toggle: Enabled" : "Toggle: Disabled",
            3.5f, COLOR_WHITE, COLOR_BLUE_HOVER);
    }

    /*if (ImGui::Checkbox("Players Wallhack", &ApplicationInfo::bCheatEnabled[CHEAT_PLAYERS_WALLHACK])) {
        AddNotify("Players Wallhack",
            ApplicationInfo::bCheatEnabled[CHEAT_PLAYERS_WALLHACK] ? "Toggle: Enabled" : "Toggle: Disabled",
            3.5f, COLOR_WHITE, COLOR_BLUE_HOVER);
    }*/

    //if (ImGui::Checkbox("No Clip", &ApplicationInfo::bCheatEnabled[CHEAT_NOCLIP])) {
    //    /*AddNotify("No Clip",
    //        ApplicationInfo::bCheatEnabled[CHEAT_NOCLIP] ? "Toggle: Enabled" : "Toggle: Disabled",
    //        3.5f, COLOR_WHITE, COLOR_BLUE_HOVER);

    //    if (!ApplicationInfo::bCheatEnabled[CHEAT_NOCLIP]) {
    //        ResetNoclip();
    //    }*/
    //}

    if (ImGui::Checkbox("SpeedHack", &ApplicationInfo::bCheatEnabled[CHEAT_PLAYERSPEED])) 
    {
        AddNotify("SpeedHack",
            ApplicationInfo::bCheatEnabled[CHEAT_NOCLIP] ? "Toggle: Enabled" : "Toggle: Disabled",
            3.5f, COLOR_WHITE, COLOR_BLUE_HOVER);
    }

    if (ApplicationInfo::bCheatEnabled[CHEAT_PLAYERSPEED]) 
    {
        if (ImGui::SliderFloat("Player Speed", &ApplicationInfo::fPlayerSpeed, 0.00f, 15.00f, "%.2f")) {}

        if (ImGui::Button("Reset Speed")) {
            ApplicationInfo::fPlayerSpeed = 1.60f;
        }
    }
   
    ImGui::TextColored(titleColor, "Player / Information");
    ImGui::Separator();

    if (!Game::isOnMission) {
        ImGui::Text("You must be in game for see it :)");
    }
    else {
        ImGui::Text("indev..");
    }
    /*else
    {
        if (Ghost::GetInfo(GhostAI::instance) && Network::GetInstance())
        {
            auto pLocalPlayer = Network::GetInstance()->GetLocalPlayer();
            if (pLocalPlayer) {
                if (pLocalPlayer) {
                    auto room = pLocalPlayer->currentRoom;
                    if (room) {
                        II::String* roomName = room->roomName;
                        if (roomName) {
                            ImGui::Text("Current room: %s", roomName->ToString().c_str());
                        }
                    }

                    ImGui::Text("Your Name: %s", Players::GetNickname(pLocalPlayer));
                }
            }
        }
    }*/
}

void Gui::RenderGhostPage(const ImVec4& titleColor) 
{
    static const std::array<std::pair<const char*, int>, 6> ghostCheats = { {
        {"Ghost Wallhack", CHEAT_GHOST_WALLHACK},
        {"Hunt notifications", CHEAT_HUNT_NOTIFIES},
        {"Ghost info show", CHEAT_GHOST_INFO_SHOW},
        {"EMF Show", CHEAT_EMF_DATA},
        {"Cursed Items Show", CHEAT_CURSED_ITEMS_SHOW},
        {"Bone Show", CHEAT_BONE_SHOW}/*,
        {"No hunting", CHEAT_NOHUNTING}*/
    } };

    ImGui::TextColored(titleColor, "Ghost / Cheats");
    ImGui::Separator();

    for (const auto& [label, cheatId] : ghostCheats) 
    {
        if (ImGui::Checkbox(label, &ApplicationInfo::bCheatEnabled[cheatId])) {
            AddNotify(label,
                ApplicationInfo::bCheatEnabled[cheatId] ? "Toggle: Enabled" : "Toggle: Disabled",
                3.5f, COLOR_WHITE, COLOR_BLUE_HOVER);
        }
    }

    ImGui::TextColored(titleColor, "Ghost / Information");
    ImGui::Separator();

    if (!Game::isOnMission) {
        ImGui::Text("You must be in game for see it :)");
    }
    else
    {
        if (Ghost::GetInfo(GhostAI::instance))
        {
            if (auto* pRoom = GetGhostCurrentRoom())
            {
                ImGui::Text("Name: %s", Ghost::GetName());
                ImGui::Text("Type: %s", Ghost::GetTypeName(Ghost::GetType()));
                ImGui::Text("State: %s", Ghost::GetStateName());
                ImGui::Text("Gender: %s", Ghost::GetSex());
                ImGui::Text("Age: %d", Ghost::GetAge());
                ImGui::Text("Room: %s", Room::GetRoomName(pRoom));
                if (Ghost::GetType() == GhostTraits::GhostType::Banshee && Ghost::GetBansheeTargetNickname()) {
                    ImGui::Text("Banshee target: %s", Ghost::GetBansheeTargetNickname());
                }

                if (Ghost::GetType() == GhostTraits::GhostType::Mimic) {
                    ImGui::Text("Mimic Type: %s", Ghost::GetTypeName(Ghost::GetMimicType()));
                }
            }
        }
    }
}

void Gui::RenderPreferencesPage(const ImVec4& titleColor) {
    static char lightName[128] = "Directional";
    static float brightness = 20.0f;
    static float range = 10.0f;
    static std::vector<std::string> lightNames;
    static int selectedLightIndex = 0;

    ImGui::TextColored(titleColor, "Preferences");
    ImGui::Separator();

    if (lightNames.empty()) {
        std::set<std::string> uniqueNames;
        auto lights = II::Light::FindAll();
        for (auto* light : lights) {
            if (light && light->GetGameObject()) {
                uniqueNames.insert(light->GetGameObject()->GetName()->ToString());
            }
        }
        lightNames.assign(uniqueNames.begin(), uniqueNames.end());
    }

    if (!lightNames.empty()) {
        if (ImGui::Combo("Light Name Selector", &selectedLightIndex, [](void* data, int idx, const char** out_text) {
            auto& vec = *static_cast<std::vector<std::string>*>(data);
            *out_text = vec[idx].c_str();
            return true;
            }, &lightNames, lightNames.size())) {
            strncpy_s(lightName, lightNames[selectedLightIndex].c_str(), sizeof(lightName));
        }
    }
    else {
        ImGui::Text("No lights found");
    }

    ImGui::Text("Brightness");
    ImGui::SameLine();
    if (ImGui::Button("-##BrightnessMinus")) brightness = max(0.0f, brightness - 0.1f);
    ImGui::SameLine();
    ImGui::SliderFloat("##Brightness", &brightness, 0.0f, 255.0f, "%.1f");
    ImGui::SameLine();
    if (ImGui::Button("+##BrightnessPlus")) brightness = min(255.0f, brightness + 0.1f);

    ImGui::Text("Range");
    ImGui::SameLine();
    if (ImGui::Button("-##RangeMinus")) range = max(0.0f, range - 0.1f);
    ImGui::SameLine();
    ImGui::SliderFloat("##Range", &range, 0.0f, 100000.0f, "%.1f");
    ImGui::SameLine();
    if (ImGui::Button("+##RangePlus")) range = min(100000.0f, range + 0.1f);

    if (ImGui::Button("Apply")) {
        SetSpecificLightBrightness(lightName, brightness, range);
    }

    if (ImGui::Button("Full Bright")) {
        ApplicationInfo::bCheatEnabled[CHEAT_FULLBRIGHT] = !ApplicationInfo::bCheatEnabled[CHEAT_FULLBRIGHT];
        ToggleFullBright(ApplicationInfo::bCheatEnabled[CHEAT_FULLBRIGHT]);
        AddNotify("Full Bright",
            ApplicationInfo::bCheatEnabled[CHEAT_FULLBRIGHT] ? "Toggle: Enabled" : "Toggle: Disabled",
            3.5f, COLOR_WHITE, COLOR_BLUE_HOVER);
    }
}
//void ApplyFullbright();
void Gui::RenderConsolePage(const ImVec4& titleColor) {
    static bool consoleInitialized = false;
    static char inputBuffer[256] = "";
    static std::vector<std::string> consoleLog;
    static std::vector<std::string> commandHistory;
    static int historyPos = -1;

    if (!consoleInitialized) {
        consoleLog.push_back("Cheat Console initialized");
        consoleLog.push_back("Type 'help' for available commands");
        consoleInitialized = true;
    }

    ImGui::TextColored(titleColor, "Console");
    ImGui::Separator();

    const float footerHeight = ImGui::GetFrameHeightWithSpacing();
    if (ImGui::BeginChild("ConsoleScroll", ImVec2(0, -footerHeight), false, ImGuiWindowFlags_HorizontalScrollbar)) {
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));

        for (const auto& log : consoleLog) {
            if (log.find("[ERROR]") != std::string::npos) {
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.4f, 0.4f, 1.0f));
            }
            else if (log.find("[WARNING]") != std::string::npos) {
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.8f, 0.4f, 1.0f));
            }
            else if (log.find("> ") != std::string::npos) {
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.8f, 1.0f, 1.0f));
            }

            ImGui::TextUnformatted(log.c_str());

            if (log.find("[ERROR]") != std::string::npos ||
                log.find("[WARNING]") != std::string::npos ||
                log.find("> ") != std::string::npos) {
                ImGui::PopStyleColor();
            }
        }

        if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
            ImGui::SetScrollHereY(1.0f);
        }

        ImGui::PopStyleVar();
    }
    ImGui::EndChild();

    

    ImGui::Separator();
    bool reclaimFocus = false;
    ImGui::PushItemWidth(-1);
    if (ImGui::InputText("##ConsoleInput", inputBuffer, sizeof(inputBuffer),
        ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackHistory,
        [](ImGuiInputTextCallbackData* data) -> int {
            if (data->EventKey == ImGuiKey_UpArrow && historyPos + 1 < commandHistory.size()) {
                historyPos++;
                strcpy_s(data->Buf, data->BufSize, commandHistory[commandHistory.size() - 1 - historyPos].c_str());
                data->BufDirty = true;
            }
            else if (data->EventKey == ImGuiKey_DownArrow && historyPos >= 0) {
                historyPos--;
                if (historyPos == -1) {
                    data->Buf[0] = '\0';
                }
                else {
                    strcpy_s(data->Buf, data->BufSize, commandHistory[commandHistory.size() - 1 - historyPos].c_str());
                }
                data->BufDirty = true;
            }
            return 0;
        })) {
        std::string command = inputBuffer;
        if (!command.empty()) {
            commandHistory.push_back(command);
            if (commandHistory.size() > 50) {
                commandHistory.erase(commandHistory.begin());
            }
            historyPos = -1;

            consoleLog.push_back("> " + command);

            if (command == "help") {
                consoleLog.push_back("Available commands:");
                consoleLog.push_back("  help - Show this help");
                consoleLog.push_back("  clear - Clear console");
                consoleLog.push_back("  tpadd x y z - Change player position offset");
                consoleLog.push_back("  ghostinfo - Show ghost info");
                consoleLog.push_back("  bp - bind current pos");
                consoleLog.push_back("  gbp - teleport to binded pos");
                consoleLog.push_back("  sct - change camera type (Game = 1, SceneView = 2, Preview = 4, VR = 8, Reflection = 16)");
            }
            else if (command == "test") {
                //ApplyFullbright();
                consoleLog.push_back("test ok");
            }
            else if (command.find("sct ") == 0) {
                int typeInt = 0;
                if (sscanf_s(command.c_str() + 4, "%d", &typeInt) == 1) {
                    if (auto camera = II::Camera::GetMain()) 
                    {
                        switch ((II::CameraType)typeInt) {
                            case II::CameraType::Game:
                            case II::CameraType::SceneView:
                            case II::CameraType::Preview:
                            case II::CameraType::VR:
                            case II::CameraType::Reflection:
                            {
                                consoleLog.push_back("[SYSTEM] Camera type updated!");
                                camera->SetCameraType((II::CameraType)typeInt);
                                break;
                            }

                            default: {
                                consoleLog.push_back("[ERROR] Invalid camera type!");
                                break;
                            }
                        }
                    }
                }
            }
            else if (command == "clear") {
                consoleLog.clear();
            }
            else if (command == "t1") {
                /*CheatManager::GetInstance()->ToggleCheat("AntiStamina");*/
                if (MapController::GetInstance())
                {
                    for (const auto player : MapController::GetInstance()->GetPlayers()) {
                        if (player) 
                        {
                            if (Network::GetInstance()) 
                            {
                                auto localPlayer = Network::GetInstance()->GetLocalPlayer();
                                if (player == localPlayer) {
                                    //Log("Local player founded on map: " + Players::GetNickname(player));
                                    //Log("players count: " + std::to_string(MapController::GetInstance()->GetPlayersCount()));
                                }
                                else {
                                    //Log("Remote player founded on map: " + Players::GetNickname(player));
                                    //Log("players count: " + std::to_string(MapController::GetInstance()->GetPlayersCount()));
                                }
                                //const char* name = Network::GetInstance()->GetPlayerName(player);
                                //Log("player founded on map: " + std::string(name));
                            }
                            /*if (Players::IsLocalPlayer(player)) {
                                Log("Local player founded on map: " + Players::GetNickname(player));
                            }
                            else {
                                Log("Remote player founded on map: " + Players::GetNickname(player));
                            }*/
                        }
                    }
                   
                }

                consoleLog.push_back("[SYSTEM] called test!");
            }
            else if (command == "t2") {
                /*CheatManager::GetInstance()->ToggleCheat("AntiStamina");*/
                if (auto network = Network::GetInstance())
                {
                    // LOCAL
                    auto localPlayer = network->GetLocalPlayer();

                    
                }
                else 
                {
                    consoleLog.push_back("[SYSTEM] network instance not found!");
                }
            }
            else if (command.find("bp") == 0) {
                auto player = UnityResolve::UnityType::GameObject::Find("PCPlayer(Clone)");
                if (player) {
                    vecBindPos = { player->GetTransform()->GetPosition().x, player->GetTransform()->GetPosition().y, player->GetTransform()->GetPosition().z };
                    consoleLog.push_back("[SYSTEM] Pos binded! (XYZ: " + std::to_string(vecBindPos.x) + ", " + std::to_string(vecBindPos.y) + ", " + std::to_string(vecBindPos.z) + ")");
                }
                else {
                    consoleLog.push_back("[ERROR] Player not found");
                }
            }
            else if (command.find("gbp") == 0) {
                float x, y, z;
                auto player = UnityResolve::UnityType::GameObject::Find("PCPlayer(Clone)");
                if (player) {
                    player->GetTransform()->SetPosition(vecBindPos);
                    consoleLog.push_back("[SYSTEM] Go to binded pos, success! (XYZ: " + std::to_string(vecBindPos.x) + ", " + std::to_string(vecBindPos.y) + ", " + std::to_string(vecBindPos.z) + ")");
                }
                else {
                    consoleLog.push_back("[ERROR] Player not found");
                }
            }
            else if (command.find("tpadd ") == 0) {
                float x, y, z;
                if (sscanf_s(command.c_str() + 6, "%f %f %f", &x, &y, &z) == 3) {
                    auto player = UnityResolve::UnityType::GameObject::Find("PCPlayer(Clone)");
                    if (player) {
                        II::Vector3 vecPlayerPos = player->GetTransform()->GetPosition();
                        player->GetTransform()->SetPosition(II::Vector3(vecPlayerPos.x + x, vecPlayerPos.y + y, vecPlayerPos.z + z));
                        consoleLog.push_back("[SYSTEM] Teleported to " + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z));
                    }
                    else {
                        consoleLog.push_back("[ERROR] Player not found");
                    }
                }
                else {
                    consoleLog.push_back("[ERROR] Usage: teleport x y z");
                }
            }
            else if (command == "fullbright") {
                ApplicationInfo::bCheatEnabled[CHEAT_FULLBRIGHT] = !ApplicationInfo::bCheatEnabled[CHEAT_FULLBRIGHT];
                ToggleFullBright(ApplicationInfo::bCheatEnabled[CHEAT_FULLBRIGHT]);
                consoleLog.push_back(ApplicationInfo::bCheatEnabled[CHEAT_FULLBRIGHT] ?
                    "[SYSTEM] Fullbright enabled" : "[SYSTEM] Fullbright disabled");
            }
            else if (command == "ghostinfo") {
                if (auto* pRoom = GetGhostCurrentRoom()) {
                    consoleLog.push_back("[GHOST] Name: " + std::string(Ghost::GetName()));
                    consoleLog.push_back("[GHOST] Type: " + std::string(Ghost::GetTypeName(Ghost::GetType())));
                    consoleLog.push_back("[GHOST] State: " + std::string(Ghost::GetStateName()));
                }
                else {
                    consoleLog.push_back("[ERROR] Not in game or ghost not found");
                }
            }
            else {
                consoleLog.push_back("[ERROR] Unknown command: " + command);
            }

            strcpy_s(inputBuffer, "");
            reclaimFocus = true;
        }
    }
    ImGui::PopItemWidth();

    if (reclaimFocus || (ImGui::IsWindowFocused() && !ImGui::IsAnyItemActive() && !ImGui::IsMouseClicked(0))) {
        ImGui::SetKeyboardFocusHere(-1);
    }

    if (ImGui::Button("Clear")) {
        consoleLog.clear();
    }
    ImGui::SameLine();
    if (ImGui::Button("Copy to Clipboard")) {
        std::string logText;
        for (const auto& log : consoleLog) {
            logText += log + "\n";
        }
        ImGui::SetClipboardText(logText.c_str());
        consoleLog.push_back("[SYSTEM] Console log copied to clipboard");
    }
}

void Gui::ProcessCheats() 
{
    if (ApplicationInfo::bCheatEnabled[CHEAT_PLAYERSPEED]) 
    {
        if (auto netInstance = Network::GetInstance())
        {
            if (auto localPlayer = netInstance->GetLocalPlayer())
            {
                if (localPlayer->firstPersonController->currentSpeed != ApplicationInfo::fPlayerSpeed) 
                {
                    if (localPlayer->firstPersonController->m_IsSprinting) 
                    {
                        localPlayer->firstPersonController->currentSpeed = 1.40f + ApplicationInfo::fPlayerSpeed;
                    }
                    else 
                    {
                        localPlayer->firstPersonController->currentSpeed = ApplicationInfo::fPlayerSpeed;
                    }
                }
            }
        }
    }
}

void Gui::GlobalDraw() 
{
    if (ApplicationInfo::bMenuActive) {
        RenderMainWindow();
    }

    DoDrawFeatures();
    notificationManager.render();
}

void Gui::RenderMainWindow() {
    ImGuiIO& io = ImGui::GetIO();
    ImVec2 windowSize(io.DisplaySize.x * 0.4925f, io.DisplaySize.y * 0.47f);
    ImVec2 windowPos((io.DisplaySize.x - windowSize.x) * 0.5f, (io.DisplaySize.y - windowSize.y) * 0.5f);

    ImGui::SetNextWindowSize(windowSize, ImGuiCond_Once);
    ImGui::SetNextWindowPos(windowPos, ImGuiCond_Once);

    if (ImGui::Begin("Phasmohook", nullptr, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse)) {
        ImGui::BeginChild("Sidebar", ImVec2(150, -1), true);
        RenderSideBar();
        ImGui::EndChild();

        ImGui::SameLine();
        ImGui::BeginChild("MainContent", ImVec2(-1, -1), true);
        RenderMainContent();
        ImGui::EndChild();
    }
    ImGui::End();
}