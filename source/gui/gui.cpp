#include "../main.h"
#include <array>
#include <unordered_map>

extern PlayerPool* pPlayerPool;
extern DNAEvidence* pBone;

NotificationManager notificationManager;

auto GetGhostFavouriteRoom() -> LevelRoom*;
auto UNITY_CALLING_CONVENTION GhostModel__Show(void* _this, bool bShow) -> void;

float defBright_Light = 0.0f;
float defRange_Light = 0.0f;

float defBright_PCAreaLight = 0.0f;
float defRange_PCAreaLight = 0.0f;

float defBright_DirectionalLight = 0.0f;
float defRange_DirectionalLight = 0.0f;

void SetSpecificLightBrightness(const std::string& lightName, float brightness, float range) {
    auto* mainCamera = II::Camera::GetMain();
    if (!mainCamera) return;
    auto lights = UnityResolve::UnityType::Light::FindAll();
    if (!lights.empty()) {
        bool found = false;
        for (auto* light : lights) {
            if (light) {
                auto* go = light->GetGameObject();
                if (go) {
                    std::string name = std::format("{}", go->GetName()->ToString());
                    if (name.find(lightName) != std::string::npos) {
                        light->SetIntensity(brightness);
                        light->SetRange(range);
                        //std::cout << "Applied brightness " << brightness << " to light: " << lightName << std::endl;
                        found = true;
                        break;
                    }
                }
            }
        }
        if (!found) {
            //std::cerr << "Light with name '" << lightName << "' not found" << std::endl;
        }
    }
    else {
        //std::cerr << "No active lights found in the scene" << std::endl;
    }
}

bool GetSpecificLightBrightness(const std::string& lightName, float* brightness, float* range) {
    auto* mainCamera = II::Camera::GetMain();
    if (!mainCamera) return false;

    auto lights = UnityResolve::UnityType::Light::FindAll();
    if (!lights.empty()) {
        for (auto* light : lights) {
            if (light) {
                auto* go = light->GetGameObject();
                if (go) {
                    std::string name = std::format("{}", go->GetName()->ToString());
                    if (name.find(lightName) != std::string::npos) {
                        *brightness = light->GetIntensity();
                        *range = light->GetRange();
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

namespace GhostUtils {
    inline void ShowGhostModelForce(bool bShow) {
        if (!Ghost::gCurrentGhost) return;
        if (void* pModel = Ghost::GetGhostModel(Ghost::gCurrentGhost)) {
            GhostModel__Show(pModel, bShow);
        }
    }
}

struct BonePair {
    II::Animator::HumanBodyBones start, end;
    const char* label;
};

constexpr std::array<BonePair, 20> bonePairs = { {
    {II::Animator::HumanBodyBones::Head, II::Animator::HumanBodyBones::Neck, "Head -> Neck"},
    {II::Animator::HumanBodyBones::Neck, II::Animator::HumanBodyBones::UpperChest, "Neck -> UpperChest"},
    {II::Animator::HumanBodyBones::UpperChest, II::Animator::HumanBodyBones::Chest, "UpperChest -> Chest"},
    {II::Animator::HumanBodyBones::Chest, II::Animator::HumanBodyBones::Spine, "Chest -> Spine"},
    {II::Animator::HumanBodyBones::Spine, II::Animator::HumanBodyBones::Hips, "Spine -> Hips"},
    {II::Animator::HumanBodyBones::LeftShoulder, II::Animator::HumanBodyBones::LeftUpperArm, "LeftShoulder -> LeftUpperArm"},
    {II::Animator::HumanBodyBones::LeftUpperArm, II::Animator::HumanBodyBones::LeftLowerArm, "LeftUpperArm -> LeftLowerArm"},
    {II::Animator::HumanBodyBones::LeftLowerArm, II::Animator::HumanBodyBones::LeftHand, "LeftLowerArm -> LeftHand"},
    {II::Animator::HumanBodyBones::RightShoulder, II::Animator::HumanBodyBones::RightUpperArm, "RightShoulder -> RightUpperArm"},
    {II::Animator::HumanBodyBones::RightUpperArm, II::Animator::HumanBodyBones::RightLowerArm, "RightUpperArm -> RightLowerArm"},
    {II::Animator::HumanBodyBones::RightLowerArm, II::Animator::HumanBodyBones::RightHand, "RightLowerArm -> RightHand"},
    {II::Animator::HumanBodyBones::LeftUpperLeg, II::Animator::HumanBodyBones::LeftLowerLeg, "LeftUpperLeg -> LeftLowerLeg"},
    {II::Animator::HumanBodyBones::LeftLowerLeg, II::Animator::HumanBodyBones::LeftFoot, "LeftLowerLeg -> LeftFoot"},
    {II::Animator::HumanBodyBones::LeftFoot, II::Animator::HumanBodyBones::LeftToes, "LeftFoot -> LeftToes"},
    {II::Animator::HumanBodyBones::RightUpperLeg, II::Animator::HumanBodyBones::RightLowerLeg, "RightUpperLeg -> RightLowerLeg"},
    {II::Animator::HumanBodyBones::RightLowerLeg, II::Animator::HumanBodyBones::RightFoot, "RightLowerLeg -> RightFoot"},
    {II::Animator::HumanBodyBones::RightFoot, II::Animator::HumanBodyBones::RightToes, "RightFoot -> RightToes"},
    {II::Animator::HumanBodyBones::LeftEye, II::Animator::HumanBodyBones::Head, "LeftEye -> Head"},
    {II::Animator::HumanBodyBones::RightEye, II::Animator::HumanBodyBones::Head, "RightEye -> Head"},
    {II::Animator::HumanBodyBones::Jaw, II::Animator::HumanBodyBones::Head, "Jaw -> Head"}
} };

ImVec4 HexToRGBA(const std::string& hex) {
    int r, g, b, a;
    sscanf_s(hex.c_str(), "%02x%02x%02x%02x", &r, &g, &b, &a);
    return ImVec4(static_cast<float>(r) / 255, static_cast<float>(g) / 255, static_cast<float>(b) / 255, static_cast<float>(a) / 255);
}

inline void DrawTextFromPosition(std::string_view text, const II::Vector3& position) {
    if (auto* camera = II::Camera::GetMain()) {
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

void Gui::ApplyStyles() {
    ImGuiStyle& style = ImGui::GetStyle();

    constexpr ImVec2 windowPadding(15.0f, 15.0f);
    constexpr ImVec2 framePadding(5.0f, 5.0f);
    constexpr ImVec2 itemSpacing(12.0f, 8.0f);
    constexpr ImVec2 itemInnerSpacing(8.0f, 6.0f);

    style.WindowPadding = windowPadding;
    style.FramePadding = ImVec2(5.0f, 4.0f);
    style.ItemSpacing = itemSpacing;
    style.ItemInnerSpacing = itemInnerSpacing;
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

char Gui::ProcessInput(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    HotKey::PotMsg(msg);

    static const std::unordered_map<WPARAM, std::pair<int, const char*>> hotkeys = {
        {VK_RETURN, {0, "Menu"}},
        {0x31, {CHEAT_PLAYER_ANTISTAMINA, "Disable Stamina"}},
        {0x32, {CHEAT_GHOST_WALLHACK, "Ghost Wallhack"}},
        {0x33, {CHEAT_HUNT_NOTIFIES, "Hunt notifications"}},
        {0x34, {CHEAT_GHOST_INFO_SHOW, "Ghost info show"}},
        {0x35, {CHEAT_EMF_DATA, "EMF Show"}},
        {0x36, {CHEAT_FULLBRIGHT, "FullBright"}}/*,
        {0x37, {CHEAT_SUPERFLASHLIGHT, "Super FlashLight"}}*/
        
        
        /*,
        {0x36, {CHEAT_GHOST_MODEL_SHOW, "Ghost Model Show"}},
        {0x37, {CHEAT_VISIBLE_GHOST_AT_HUNT, "Visible Ghost Hunt"}}*/
    };

    switch (msg) {
    case WM_KEYDOWN:
        if (wParam == VK_RETURN) {
            ApplicationInfo::bMenuActive = !ApplicationInfo::bMenuActive;
            ShowCursor(ApplicationInfo::bMenuActive);
        }
        else if (auto it = hotkeys.find(wParam); it != hotkeys.end()) {
            const auto& [cheatId, title] = it->second;
            ApplicationInfo::bCheatEnabled[cheatId] = !ApplicationInfo::bCheatEnabled[cheatId];
            AddNotify(title,
                ApplicationInfo::bCheatEnabled[cheatId] ? "Toggle: Enabled" : "Toggle: Disabled",
                3.5f, COLOR_WHITE, COLOR_BLUE_HOVER);
           /* if (cheatId == CHEAT_GHOST_MODEL_SHOW) {
                GhostUtils::ShowGhostModelForce(ApplicationInfo::bCheatEnabled[cheatId]);
            }*/

           /* if (cheatId == CHEAT_SUPERFLASHLIGHT)
            {
                if (defBright_Light == 0.0f && defRange_Light == 0.0f)
                    GetSpecificLightBrightness("Light", &defBright_Light, &defRange_Light);

                if (ApplicationInfo::bCheatEnabled[CHEAT_SUPERFLASHLIGHT]) {
                    SetSpecificLightBrightness("Light", 255.0f, 50.0f);
                }
                else
                {
                    SetSpecificLightBrightness("Light", defBright_Light, defRange_Light);
                }
            }*/

            if (cheatId == CHEAT_FULLBRIGHT)
            {
                if (defBright_Light == 0.0f && defRange_Light == 0.0f)
                    GetSpecificLightBrightness("Light", &defBright_Light, &defRange_Light);

                if (defBright_PCAreaLight == 0.0f && defRange_PCAreaLight == 0.0f)
                    GetSpecificLightBrightness("PCAreaLight", &defBright_PCAreaLight, &defRange_PCAreaLight);

                if (defBright_DirectionalLight == 0.0f && defRange_DirectionalLight == 0.0f)
                    GetSpecificLightBrightness("DirectionalLight", &defBright_DirectionalLight, &defRange_DirectionalLight);

                if (ApplicationInfo::bCheatEnabled[CHEAT_FULLBRIGHT]) {
                    SetSpecificLightBrightness("Light", 255.0f, 50.0f);
                    SetSpecificLightBrightness("PCAreaLight", 255.0f, 50.0f);
                    SetSpecificLightBrightness("Directional Light", 0.1f, 0.1f);
                }
                else
                {
                    SetSpecificLightBrightness("Light", defBright_Light, defRange_Light);
                    SetSpecificLightBrightness("PCAreaLight", defBright_PCAreaLight, defRange_PCAreaLight);
                    SetSpecificLightBrightness("Directiona Light", defBright_DirectionalLight, defRange_DirectionalLight);
                }
            }
        }
        break;
    case WM_CLOSE:
        if (MessageBox(nullptr, L"Exit?", L"Confirmation", MB_YESNO | MB_ICONQUESTION) == IDYES) {
            std::exit(0);
        }
        break;
    }

    if (ApplicationInfo::bMenuActive) {
        ClipCursor(nullptr);
        return static_cast<char>(ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam));
    }
    ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
    return 1;
}

void Gui::AddNotify(const std::string& title, const std::string& message, float displayTime, ImColor textColor, ImColor bgColor) {
    notificationManager.addNotification(title, message, displayTime, 10.0f, textColor, bgColor);
}

static std::chrono::steady_clock::time_point lastFrameTime;
void Gui::updateLoop(std::chrono::steady_clock::time_point time) {    
    lastFrameTime = time;
}

inline void DrawBoneConnections(II::Animator* pAnimator, float distance) {
    if (!pAnimator || !II::Camera::GetMain()) return;

    auto* drawList = ImGui::GetBackgroundDrawList();
    auto* camera = II::Camera::GetMain();

    for (const auto& pair : bonePairs) {
        if (auto* start = pAnimator->GetBoneTransform(pair.start)) {
            if (auto* end = pAnimator->GetBoneTransform(pair.end)) {
                auto startPoint = camera->WorldToScreenPoint(start->GetPosition(), UnityResolve::UnityType::Camera::Eye::Mono);
                auto endPoint = camera->WorldToScreenPoint(end->GetPosition(), UnityResolve::UnityType::Camera::Eye::Mono);

                if (startPoint.z > 0 && endPoint.z > 0) {
                    startPoint.y = ApplicationInfo::screenHeight - startPoint.y;
                    endPoint.y = ApplicationInfo::screenHeight - endPoint.y;
                    drawList->AddLine(
                        { startPoint.x, startPoint.y },
                        { endPoint.x, endPoint.y },
                        IM_COL32(255, 255, 255, 255),
                        1.5f
                    );
                }
            }
        }
    }
}

inline auto CalculateEntityData(auto* entity) -> std::tuple<II::Vector3, II::Vector3, float, float> {
    if (!entity || !II::Camera::GetMain()) return {};

    auto* transform = entity->GetTransform();
    auto* camera = II::Camera::GetMain();

    auto entityPos = transform->GetPosition();
    auto cameraPos = camera->GetTransform()->GetPosition();
    float distance = (entityPos - cameraPos).Length();
    float scale = transform->GetLocalScale().Length();

    return { entityPos, cameraPos, distance - scale, scale * 100.0f };
}

void ManageEMFList() {
    if (!Ghost::gCurrentGhost || !ApplicationInfo::bCheatEnabled[CHEAT_EMF_DATA]) return;

    auto now = std::chrono::steady_clock::now();
    std::vector<EMF*> toRemove;

    for (auto it = Ghost::emfData.begin(); it != Ghost::emfData.end();) {
        auto& [emf, timestamp] = *it;
        if (!emf) {
            it = Ghost::emfData.erase(it);
            continue;
        }

        auto seconds = std::chrono::duration_cast<std::chrono::seconds>(now - timestamp).count();
        FOR_EACH_COMPONENT(emf, II::Transform, "UnityEngine.CoreModule.dll", "Transform", pTransform) {
            if (pTransform) {
                DrawTextFromPosition(std::format("EMF ({})", 20 - seconds), pTransform->GetPosition());
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

void ManageBoneEv() {
    if (Ghost::gCurrentGhost && pBone) {
        if (auto* boneObj = pBone->GetGameObject()) {
            if (auto* transform = boneObj->GetTransform()) {
                DrawTextFromPosition("Bone", transform->GetPosition());
            }
        }
    }
}

void Gui::DoDrawFeatures() {
    auto currentFrameTime = std::chrono::high_resolution_clock::now();
    float deltaTime = std::chrono::duration<float>(currentFrameTime - lastFrameTime).count();
    lastFrameTime = currentFrameTime;

    ImGui::GetIO().DeltaTime = deltaTime;

    static bool bInjected = false;
    if (!bInjected) {
        AddNotify("Phasmohook", "Successfully injected!\n\nPress 'Enter' key for Open Menu!",
            3.5f, COLOR_WHITE, COLOR_DARK_BLUE);
        bInjected = true;
    }

    if (!Game::isOnMission || !Ghost::gCurrentGhost) return;

    if (ApplicationInfo::bCheatEnabled[CHEAT_HUNT_NOTIFIES]) {
        static bool bHunting = false;
        bool isHunting = Ghost::IsHunting();

        if (isHunting != bHunting) {
            if (isHunting) {
                AddNotify("[!] HUNTING START", "The ghost has started the hunt",
                    3.5f, COLOR_WHITE, COLOR_RED);
            }
            else {
                if (ApplicationInfo::bCheatEnabled[CHEAT_VISIBLE_GHOST_AT_HUNT]) {
                    GhostUtils::ShowGhostModelForce(false);
                }
                AddNotify("[!] HUNTING END", "The ghost stopped the hunt",
                    3.5f, COLOR_WHITE, COLOR_BLUE_HOVER);
            }
            bHunting = isHunting;
        }
    }

#ifdef __USE__PLAYER_POOLS
    if (ApplicationInfo::bCheatEnabled[CHEAT_PLAYERS_WALLHACK] && II::Camera::GetMain()) {
        for (int i = 0; i < MAX_PLAYERS; ++i) {
            if (auto* pPlayer = pPlayerPool->GetPlayer(i)) {
                if (auto* playerObj = pPlayer->GetPlayer()) {
                    if (auto* name = pPlayer->GetName()) {
                        if (!pPlayer->IsLocalPlayer() && Players::IsValidData(playerObj)) {
                            if (auto* transform = playerObj->GetTransform()) {
                                DrawTextFromPosition(name, transform->GetPosition());
                            }
                        }
                    }
                }
            }
        }
    }
#endif

    ManageEMFList();
    if (Game::IsSinglePlayer()) ManageBoneEv();

    if (ApplicationInfo::bCheatEnabled[CHEAT_GHOST_WALLHACK]) {
        auto [ghostPos, cameraPos, distance, boxSizeMax] = CalculateEntityData(Ghost::gCurrentGhost);
        FOR_EACH_COMPONENT(Ghost::gCurrentGhost, II::Animator, "UnityEngine.AnimationModule.dll", "Animator", pAnimator) {
            if (pAnimator) DrawBoneConnections(pAnimator, distance);
        }
    }

    if (ApplicationInfo::bCheatEnabled[CHEAT_GHOST_INFO_SHOW])
    {
        if (Ghost::GetInfo(Ghost::gCurrentGhost))
        {
            auto DrawOutlinedText = [](const char* label, const char* text, ImVec2 position, ImU32 outlineColor = ImColor(0, 0, 0, 255), ImU32 textColor = ImColor(255, 255, 255, 255)) {
                auto drawList = ImGui::GetWindowDrawList();

                ImVec2 positions[] = {
                    { position.x - 1, position.y },     // Left
                    { position.x + 1, position.y },     // Right
                    { position.x, position.y - 1 },     // Up
                    { position.x, position.y + 1 },     // Down
                    { position.x - 1, position.y - 1 }, // Top-left diagonal
                    { position.x + 1, position.y - 1 }, // Top-right diagonal
                    { position.x - 1, position.y + 1 }, // Bottom-left diagonal
                    { position.x + 1, position.y + 1 }, // Bottom-right diagonal
                };

                // Draw outline for the label
                for (const auto& outlinePos : positions) {
                    drawList->AddText(outlinePos, outlineColor, label);
                }

                drawList->AddText(position, textColor, label);

                ImVec2 textPos = { position.x + 100, position.y };

                for (const auto& outlinePos : positions) {
                    drawList->AddText({ outlinePos.x + 100, outlinePos.y }, outlineColor, text);
                }

                drawList->AddText(textPos, textColor, text);
                };



            LevelRoom* pRoom = GetGhostFavouriteRoom();
            if (pRoom) {
                ImGui::SetNextWindowSize(ImVec2(1920, 1080), ImGuiCond_Once);

                ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoTitleBar |
                    ImGuiWindowFlags_NoResize |
                    ImGuiWindowFlags_NoCollapse |
                    ImGuiWindowFlags_NoBackground;

                // NOTE: dragging fix on disabled menu
                if (ApplicationInfo::bMenuActive) {
                    windowFlags &= ~ImGuiWindowFlags_NoMove;
                } else {
                    windowFlags |= ImGuiWindowFlags_NoMove;
                }

                ImGui::Begin("#ghostInformation", nullptr, windowFlags);

                ImVec2 pos = ImGui::GetCursorScreenPos();

                DrawOutlinedText("Ghost info: ", "", pos);

                pos.y += ImGui::GetTextLineHeight();

                DrawOutlinedText("Type: ", Ghost::GetTypeName(), pos);

                pos.y += ImGui::GetTextLineHeight();

                if (Ghost::IsHunting()) {
                    DrawOutlinedText("State: ", Ghost::GetStateName(), pos, ImColor(0, 0, 0, 255), ImColor(255, 0, 0, 255));
                }
                else DrawOutlinedText("State: ", Ghost::GetStateName(), pos);

                pos.y += ImGui::GetTextLineHeight();


                DrawOutlinedText("Rroom: ", pRoom ? Room::GetRoomName(pRoom) : "", pos);

                pos.y += ImGui::GetTextLineHeight();

                DrawOutlinedText("Temperature: ", pRoom ? std::format("{}", Room::GetRoomTemperature(pRoom)).c_str() : "None", pos);

                pos.y += ImGui::GetTextLineHeight();

                DrawOutlinedText("Name: ", Ghost::GetName(), pos);

                pos.y += ImGui::GetTextLineHeight();

                bool bGhostEvent = Ghost::IsGhostEvent();
                DrawOutlinedText("Event: ", std::format("{}", bGhostEvent ? "Yes" : "No").c_str(), pos, ImColor(0, 0, 0, 255), bGhostEvent ? ImColor(0, 255, 0, 255) : ImColor(255, 255, 255, 255));
                
                pos.y += ImGui::GetTextLineHeight();

                bool bIncest = Ghost::IncenseEffect();
                DrawOutlinedText("Incense: ", std::format("{}", bIncest ? "Yes" : "No").c_str(), pos, ImColor(0, 0, 0, 255), bIncest ? ImColor(0, 255, 0, 255) : ImColor(255, 255, 255, 255));

                if (void* pLocal = PhotonHelper::GetLocalPlayer())
                {
                    II::String* name = PhotonHelper::GetRPNickName(pLocal);
                    if (name) {
                        pos.y += ImGui::GetTextLineHeight();
                        DrawOutlinedText("Your name: ", std::format("{}", name->ToString()).c_str(), pos);
                    }
                }

                /*pos.y += ImGui::GetTextLineHeight();

               
                char bufferState[0xFF]{};
                sprintf(bufferState, "%s", Ghost::GetHexBytes(0xE0, 0xFF).c_str());
                DrawOutlinedText("GhostAI hex: ", bufferState, pos);*/

                ImGui::End();
            }
        }
    }
}

void Gui::RenderSideBar() {
    static constexpr float buttonHeight = 45.0f;
    static const std::array<std::pair<const char*, int>, 3> buttons = { {
        {"Player", 0}, {"Ghost", 1}, {"Preferences", 2}
    } };

    for (const auto& [label, page] : buttons) {
        if (selectedPage == page) ImGui::PushStyleColor(ImGuiCol_Button, COLOR_RED);
        if (ImGui::Button(label, ImVec2(-1, buttonHeight))) selectedPage = page;
        if (selectedPage == page) ImGui::PopStyleColor();
    }
}

#include <set>
std::vector<std::string> GetUniqueLightNames() {
    std::set<std::string> uniqueNames;
    std::vector<std::string> result;

    auto* mainCamera = II::Camera::GetMain();
    if (mainCamera) {
        auto lights = II::Light::FindAll();
        if (!lights.empty()) {
            for (auto* light : lights) {
                if (light) {
                    auto* obj = light->GetGameObject();
                    if (obj) {
                        std::string stringObjName = std::format("{}", obj->GetName()->ToString());
                        uniqueNames.insert(stringObjName);
                    }
                }
            }
        }
    }

    result.assign(uniqueNames.begin(), uniqueNames.end());
    return result;
}

void Gui::RenderMainContent() {
    static const std::array<std::pair<const char*, int>, 4> ghostCheats = { {
           {"Ghost Wallhack", CHEAT_GHOST_WALLHACK},
           {"Hunt notifications", CHEAT_HUNT_NOTIFIES},
           {"Ghost info show", CHEAT_GHOST_INFO_SHOW},
           {"EMF Show", CHEAT_EMF_DATA}
           
           /*,
           {"Ghost Model Show", CHEAT_GHOST_MODEL_SHOW},
           {"Visible Ghost Hunt", CHEAT_VISIBLE_GHOST_AT_HUNT}*/
       } };

    static const ImVec4 titleColor(0.7f, 0.9f, 0.7f, 1.0f);

    static char lightName[128] = "Directional";
    static float brightness = 20.0f;
    static float range = 10.0f;

    std::vector<std::string> lightNames = GetUniqueLightNames();
    static int selectedLightIndex = 0;

    switch (selectedPage) {
    case 0: // Player
        ImGui::TextColored(titleColor, "Game Player / Cheats");
        ImGui::Separator();
        if (ImGui::Checkbox("Disable Stamina", &ApplicationInfo::bCheatEnabled[CHEAT_PLAYER_ANTISTAMINA])) {
            AddNotify("Disable Stamina",
                ApplicationInfo::bCheatEnabled[CHEAT_PLAYER_ANTISTAMINA] ? "Toggle: Enabled" : "Toggle: Disabled",
                3.5f, COLOR_WHITE, COLOR_BLUE_HOVER);
        }

#ifdef __USE__PLAYER_POOLS
        ImGui::TextColored(titleColor, "Online Players");
        ImGui::Separator();
        for (int i = 0; i < MAX_PLAYERS; ++i) {
            if (auto* pPlayer = pPlayerPool->GetPlayer(i)) {
                if (auto* player = pPlayer->GetPlayer()) {
                    if (auto* name = pPlayer->GetName()) {
                        if (Players::IsValidData(player)) {
                            ImGui::Text("[%d] %s %s",
                                pPlayerPool->GetID(player),
                                name,
                                pPlayer->IsLocalPlayer() ? "(YOU)" : "");
                        }
                    }
                }
            }
        }
#endif
        break;

    case 1: // Ghost
        ImGui::TextColored(titleColor, "Ghost / Cheats");
        ImGui::Separator();

        for (const auto& [label, cheatId] : ghostCheats) {
            if (ImGui::Checkbox(label, &ApplicationInfo::bCheatEnabled[cheatId])) {
                AddNotify(label,
                    ApplicationInfo::bCheatEnabled[cheatId] ? "Toggle: Enabled" : "Toggle: Disabled",
                    3.5f, COLOR_WHITE, COLOR_BLUE_HOVER);
                if (cheatId == CHEAT_GHOST_MODEL_SHOW) {
                    GhostUtils::ShowGhostModelForce(ApplicationInfo::bCheatEnabled[cheatId]);
                }
            }
        }

        ImGui::TextColored(titleColor, "Ghost / Information");
        ImGui::Separator();

        if (!Game::isOnMission) {
            ImGui::Text("You must be in game for see it :)");
        }
        else if (auto* pRoom = GetGhostFavouriteRoom()) {
            ImGui::Text("Name: %s", Ghost::GetName());
            ImGui::Text("Type: %s", Ghost::GetTypeName());
            ImGui::Text("State: %s", Ghost::GetStateName());
            ImGui::Text("Gender: %s", Ghost::GetSex());
            ImGui::Text("Age: %d", Ghost::GetAge());
            ImGui::Text("Fav. Room: %s", Room::GetRoomName(pRoom));
        }
        break;

    case 2: // Preferences
        ImGui::TextColored(titleColor, "Preferences");
        ImGui::Separator();

        if (!lightNames.empty()) {
            if (ImGui::Combo("Light Name Selector", &selectedLightIndex, [](void* data, int idx, const char** out_text) {
                auto& vec = *static_cast<std::vector<std::string>*>(data);
                *out_text = vec[idx].c_str();
                return true;
                }, &lightNames, lightNames.size())) {
                strncpy_s(lightName, lightNames[selectedLightIndex].c_str(), sizeof(lightName) - 1);
                lightName[sizeof(lightName) - 1] = '\0'; 
            }
        }
        else {
            ImGui::Text("No lights found");
        }

        ImGui::InputText("Light Name", lightName, sizeof(lightName));

        ImGui::Text("Brightness");
        ImGui::SameLine();
        if (ImGui::Button("-##BrightnessMinus")) {
            brightness -= 0.1f;
            if (brightness < 0.0f) brightness = 0.0f; 
        }
        if (ImGui::IsItemActive() && ImGui::GetIO().MouseDownDuration[0] > 0.1f) {
            brightness -= 0.1f;
            if (brightness < 0.0f) brightness = 0.0f;
        }
        ImGui::SameLine();
        ImGui::SliderFloat("##Brightness", &brightness, 0.0f, 255.0f, "%.1f");
        ImGui::SameLine();
        if (ImGui::Button("+##BrightnessPlus")) {
            brightness += 0.1f;
            if (brightness > 255.0f) brightness = 255.0f;
        }
        if (ImGui::IsItemActive() && ImGui::GetIO().MouseDownDuration[0] > 0.1f) {
            brightness += 0.1f;
            if (brightness > 255.0f) brightness = 255.0f;
        }

        ImGui::Text("Range");
        ImGui::SameLine();
        if (ImGui::Button("-##RangeMinus")) {
            range -= 0.1f;
            if (range < 0.0f) range = 0.0f; 
        }
        if (ImGui::IsItemActive() && ImGui::GetIO().MouseDownDuration[0] > 0.1f) { 
            range -= 0.1f;
            if (range < 0.0f) range = 0.0f;
        }
        ImGui::SameLine();
        ImGui::SliderFloat("##Range", &range, 0.0f, 100000.0f, "%.1f");
        ImGui::SameLine();
        if (ImGui::Button("+##RangePlus")) {
            range += 0.1f;
            if (range > 100000.0f) range = 100000.0f;
        }
        if (ImGui::IsItemActive() && ImGui::GetIO().MouseDownDuration[0] > 0.1f) {
            range += 0.1f;
            if (range > 100000.0f) range = 100000.0f;
        }

        if (ImGui::Button("Apply")) {
            SetSpecificLightBrightness(lightName, brightness, range);
        }

       /* if (ImGui::Button("Super FlashLight")) 
        {
            if(defBright_Light == 0.0f && defRange_Light == 0.0f)
                GetSpecificLightBrightness("Light", &defBright_Light, &defRange_Light);

            ApplicationInfo::bCheatEnabled[CHEAT_SUPERFLASHLIGHT] = !ApplicationInfo::bCheatEnabled[CHEAT_SUPERFLASHLIGHT];
            if (ApplicationInfo::bCheatEnabled[CHEAT_SUPERFLASHLIGHT]) {
                SetSpecificLightBrightness("Light", 255.0f, 50.0f);
            }
            else 
            {
                SetSpecificLightBrightness("Light", defBright_Light, defRange_Light);
            }

            AddNotify("Super FlashLight",
                ApplicationInfo::bCheatEnabled[CHEAT_SUPERFLASHLIGHT] ? "Toggle: Enabled" : "Toggle: Disabled",
                3.5f, COLOR_WHITE, COLOR_BLUE_HOVER);
        }*/

        if (ImGui::Button("Full Bright"))
        {
            ApplicationInfo::bCheatEnabled[CHEAT_FULLBRIGHT] = !ApplicationInfo::bCheatEnabled[CHEAT_FULLBRIGHT];

            if (defBright_Light == 0.0f && defRange_Light == 0.0f)
                GetSpecificLightBrightness("Light", &defBright_Light, &defRange_Light);

            if (defBright_PCAreaLight == 0.0f && defRange_PCAreaLight == 0.0f)
                GetSpecificLightBrightness("PCAreaLight", &defBright_PCAreaLight, &defRange_PCAreaLight);

            if (defBright_DirectionalLight == 0.0f && defRange_DirectionalLight == 0.0f)
                GetSpecificLightBrightness("DirectionalLight", &defBright_DirectionalLight, &defRange_DirectionalLight);

            if (ApplicationInfo::bCheatEnabled[CHEAT_FULLBRIGHT]) {
                SetSpecificLightBrightness("Light", 255.0f, 50.0f);
                SetSpecificLightBrightness("PCAreaLight", 255.0f, 50.0f);
                SetSpecificLightBrightness("Directional Light", 0.1f, 0.1f);
            }
            else
            {
                SetSpecificLightBrightness("Light", defBright_Light, defRange_Light);
                SetSpecificLightBrightness("PCAreaLight", defBright_PCAreaLight, defRange_PCAreaLight);
                SetSpecificLightBrightness("Directiona Light", defBright_DirectionalLight, defRange_DirectionalLight);
            }

            AddNotify("Super FlashLight",
                ApplicationInfo::bCheatEnabled[CHEAT_FULLBRIGHT] ? "Toggle: Enabled" : "Toggle: Disabled",
                3.5f, COLOR_WHITE, COLOR_BLUE_HOVER);
        }

        break;

    default:
        ImGui::TextColored(titleColor, "About");
        ImGui::Separator();
        ImGui::Text("PHASMOHOOK - RELEASE - 1.2 - BLACK\nThanks for download\nBy 0xcds4r");
        break;
    }
}

void Gui::GlobalDraw() {
    RenderMainWindow();
    DoDrawFeatures();
    notificationManager.render();
}

void Gui::RenderMainWindow() {
    if (!ApplicationInfo::bMenuActive) return;

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