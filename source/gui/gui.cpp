#include "../main.h"

extern PlayerPool* pPlayerPool;

NotificationManager notificationManager;
auto GetGhostFavouriteRoom() -> LevelRoom*;

auto UNITY_CALLING_CONVENTION GhostModel__Show(void* _this, bool bShow) -> void;

void ShowGhostModelForce(bool bShow) {
    if (!Ghost::gCurrentGhost) {
        return;
    }
    void* pModel = Ghost::GetGhostModel(Ghost::gCurrentGhost);
    if (!pModel) {
        return;
    }

    GhostModel__Show(pModel, bShow);
}

const std::vector<std::tuple<II::Animator::HumanBodyBones, II::Animator::HumanBodyBones, const char*>> bonePairs = {
    // Head and spine
    { II::Animator::HumanBodyBones::Head, II::Animator::HumanBodyBones::Neck, "Head -> Neck" },
    { II::Animator::HumanBodyBones::Neck, II::Animator::HumanBodyBones::UpperChest, "Neck -> UpperChest" },
    { II::Animator::HumanBodyBones::UpperChest, II::Animator::HumanBodyBones::Chest, "UpperChest -> Chest" },
    { II::Animator::HumanBodyBones::Chest, II::Animator::HumanBodyBones::Spine, "Chest -> Spine" },
    { II::Animator::HumanBodyBones::Spine, II::Animator::HumanBodyBones::Hips, "Spine -> Hips" },

    // Left arm
    { II::Animator::HumanBodyBones::LeftShoulder, II::Animator::HumanBodyBones::LeftUpperArm, "LeftShoulder -> LeftUpperArm" },
    { II::Animator::HumanBodyBones::LeftUpperArm, II::Animator::HumanBodyBones::LeftLowerArm, "LeftUpperArm -> LeftLowerArm" },
    { II::Animator::HumanBodyBones::LeftLowerArm, II::Animator::HumanBodyBones::LeftHand, "LeftLowerArm -> LeftHand" },

    // Right arm
    { II::Animator::HumanBodyBones::RightShoulder, II::Animator::HumanBodyBones::RightUpperArm, "RightShoulder -> RightUpperArm" },
    { II::Animator::HumanBodyBones::RightUpperArm, II::Animator::HumanBodyBones::RightLowerArm, "RightUpperArm -> RightLowerArm" },
    { II::Animator::HumanBodyBones::RightLowerArm, II::Animator::HumanBodyBones::RightHand, "RightLowerArm -> RightHand" },

    // Left leg
    { II::Animator::HumanBodyBones::LeftUpperLeg, II::Animator::HumanBodyBones::LeftLowerLeg, "LeftUpperLeg -> LeftLowerLeg" },
    { II::Animator::HumanBodyBones::LeftLowerLeg, II::Animator::HumanBodyBones::LeftFoot, "LeftLowerLeg -> LeftFoot" },
    { II::Animator::HumanBodyBones::LeftFoot, II::Animator::HumanBodyBones::LeftToes, "LeftFoot -> LeftToes" },

    // Right leg
    { II::Animator::HumanBodyBones::RightUpperLeg, II::Animator::HumanBodyBones::RightLowerLeg, "RightUpperLeg -> RightLowerLeg" },
    { II::Animator::HumanBodyBones::RightLowerLeg, II::Animator::HumanBodyBones::RightFoot, "RightLowerLeg -> RightFoot" },
    { II::Animator::HumanBodyBones::RightFoot, II::Animator::HumanBodyBones::RightToes, "RightFoot -> RightToes" },

    // Eyes and jaw
    { II::Animator::HumanBodyBones::LeftEye, II::Animator::HumanBodyBones::Head, "LeftEye -> Head" },
    { II::Animator::HumanBodyBones::RightEye, II::Animator::HumanBodyBones::Head, "RightEye -> Head" },
    { II::Animator::HumanBodyBones::Jaw, II::Animator::HumanBodyBones::Head, "Jaw -> Head" }
};

ImVec4 HexToRGBA(const std::string& hex) {
    int r, g, b, a;
    sscanf_s(hex.c_str(), "%02x%02x%02x%02x", &r, &g, &b, &a);
    return ImVec4(static_cast<float>(r) / 255, static_cast<float>(g) / 255, static_cast<float>(b) / 255, static_cast<float>(a) / 255);
}

void DrawTextFromPosition(const char* text, II::Vector3 position)
{
    auto pMainCamera = II::Camera::GetMain();
    if (!pMainCamera) return;
    if (position.x == 0.0f && position.y == 0.0f && position.z == 0.0f) return;

    auto point = pMainCamera->WorldToScreenPoint(position, UnityResolve::UnityType::Camera::Eye::Mono);

    if (point.z > 0) {
        point.y = ApplicationInfo::screenHeight - point.y;

        if (point.x >= 0 && point.x < ApplicationInfo::screenWidth && point.y >= 0 && point.y < ApplicationInfo::screenHeight) {

            ImVec2 textSize = ImGui::CalcTextSize(text);

            float centeredX = point.x - textSize.x / 2.0f;
            float centeredY = point.y - textSize.y / 2.0f;

            ImGui::GetBackgroundDrawList()->AddText({ centeredX, centeredY }, ImColor(255, 255, 255, 255), text);
        }
    }
}

void Gui::ApplyStyles() {
    Log("Phasmohook applying gui styles..");

    auto& styles = ImGui::GetStyle();

    auto colors = styles.Colors;

    // Настройки стилей
    styles.WindowPadding = ImVec2(15, 15);
    styles.WindowRounding = 15.0f;
    styles.FramePadding = ImVec2(5, 5);
    styles.ItemSpacing = ImVec2(12, 8);
    styles.ItemInnerSpacing = ImVec2(8, 6);
    styles.IndentSpacing = 25.0f;
    styles.ScrollbarSize = 15.0f;
    styles.ScrollbarRounding = 15.0f;
    styles.GrabMinSize = 15.0f;
    styles.GrabRounding = 7.0f;
    styles.ChildRounding = 8.0f;
    styles.ChildBorderSize = 1.0;
    styles.FramePadding = ImVec2(5.0, 4.0);
    styles.FrameRounding = 6.0f;
    styles.WindowTitleAlign = ImVec2(0.5, 0.5);
    styles.AntiAliasedFill = true;
    styles.AntiAliasedLines = true;
    styles.AntiAliasedLinesUseTex = true;

    // Настройки цветов
    colors[ImGuiCol_Text] = ImVec4(0.95f, 0.96f, 0.98f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.36f, 0.42f, 0.47f, 1.00f);
    colors[ImGuiCol_WindowBg] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
    colors[ImGuiCol_ChildBg] = ImVec4(0.15f, 0.18f, 0.22f, 1.00f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
    colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.12f, 0.20f, 0.28f, 1.00f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.09f, 0.12f, 0.14f, 1.00f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.09f, 0.12f, 0.14f, 0.65f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.08f, 0.10f, 0.12f, 1.00f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.15f, 0.18f, 0.22f, 1.00f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.39f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.18f, 0.22f, 0.25f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.09f, 0.21f, 0.31f, 1.00f);
    //colors[ImGuiCol_Combo] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
    colors[ImGuiCol_CheckMark] = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.37f, 0.61f, 1.00f, 1.00f);
    colors[ImGuiCol_Button] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
    colors[ImGuiCol_ButtonHovered] = COLOR_RED;
    colors[ImGuiCol_ButtonActive] = COLOR_RED;
    colors[ImGuiCol_Header] = ImVec4(0.20f, 0.25f, 0.29f, 0.55f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    //colors[ImGuiCol_CloseButton] = ImVec4(0.40f, 0.39f, 0.38f, 0.16f);
    //colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.40f, 0.39f, 0.38f, 0.39f);
    //colors[ImGuiCol_CloseButtonActive] = ImVec4(0.40f, 0.39f, 0.38f, 1.00f);
    colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
    //colors[ImGuiCol_ModalWindowDarkening] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);
}

extern auto ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT;
char Gui::ProcessInput(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    HotKey::PotMsg(msg);

    switch (msg) {
    case WM_KEYUP:
        break;
    case WM_KEYDOWN:
        if (wParam == VK_RETURN) {
            ApplicationInfo::bMenuActive = !ApplicationInfo::bMenuActive;
            ShowCursor(ApplicationInfo::bMenuActive);
        }

        if (wParam == 0x31) {
            ApplicationInfo::bCheatEnabled[CHEAT_PLAYER_ANTISTAMINA] = !ApplicationInfo::bCheatEnabled[CHEAT_PLAYER_ANTISTAMINA];
            Gui::AddNotify("Disable Stamina", ApplicationInfo::bCheatEnabled[CHEAT_PLAYER_ANTISTAMINA] ? "Toggle: Enabled" : "Toggle: Disabled", 3.5f, COLOR_WHITE, COLOR_BLUE_HOVER);
        }

        /*if (wParam == 0x33) {
            ApplicationInfo::bCheatEnabled[CHEAT_PLAYERS_WALLHACK] = !ApplicationInfo::bCheatEnabled[CHEAT_PLAYERS_WALLHACK];
            Gui::AddNotify("Players Wallhack", ApplicationInfo::bCheatEnabled[CHEAT_PLAYERS_WALLHACK] ? "Toggle: Enabled" : "Toggle: Disabled", 3.5f, COLOR_WHITE, COLOR_BLUE_HOVER);
        }*/

        if (wParam == 0x32) {
            ApplicationInfo::bCheatEnabled[CHEAT_GHOST_WALLHACK] = !ApplicationInfo::bCheatEnabled[CHEAT_GHOST_WALLHACK];
            Gui::AddNotify("Ghost Wallhack", ApplicationInfo::bCheatEnabled[CHEAT_GHOST_WALLHACK] ? "Toggle: Enabled" : "Toggle: Disabled", 3.5f, COLOR_WHITE, COLOR_BLUE_HOVER);
        }

        if (wParam == 0x33) {
            ApplicationInfo::bCheatEnabled[CHEAT_HUNT_NOTIFIES] = !ApplicationInfo::bCheatEnabled[CHEAT_HUNT_NOTIFIES];
            Gui::AddNotify("Hunt notifications", ApplicationInfo::bCheatEnabled[CHEAT_HUNT_NOTIFIES] ? "Toggle: Enabled" : "Toggle: Disabled", 3.5f, COLOR_WHITE, COLOR_BLUE_HOVER);
        }

        if (wParam == 0x34) {
            ApplicationInfo::bCheatEnabled[CHEAT_GHOST_INFO_SHOW] = !ApplicationInfo::bCheatEnabled[CHEAT_GHOST_INFO_SHOW];
            Gui::AddNotify("Ghost info show", ApplicationInfo::bCheatEnabled[CHEAT_GHOST_INFO_SHOW] ? "Toggle: Enabled" : "Toggle: Disabled", 3.5f, COLOR_WHITE, COLOR_BLUE_HOVER);
        }

        if (wParam == 0x35) {
            ApplicationInfo::bCheatEnabled[CHEAT_EMF_DATA] = !ApplicationInfo::bCheatEnabled[CHEAT_EMF_DATA];
            Gui::AddNotify("EMF Show", ApplicationInfo::bCheatEnabled[CHEAT_EMF_DATA] ? "Toggle: Enabled" : "Toggle: Disabled", 3.5f, COLOR_WHITE, COLOR_BLUE_HOVER);
        }

        // CHEAT_GHOST_MODEL_SHOW
        if (wParam == 0x36) {
            ApplicationInfo::bCheatEnabled[CHEAT_GHOST_MODEL_SHOW] = !ApplicationInfo::bCheatEnabled[CHEAT_GHOST_MODEL_SHOW];
            Gui::AddNotify("Ghost Model Show", ApplicationInfo::bCheatEnabled[CHEAT_GHOST_MODEL_SHOW] ? "Toggle: Enabled" : "Toggle: Disabled", 3.5f, COLOR_WHITE, COLOR_BLUE_HOVER);
            ShowGhostModelForce(ApplicationInfo::bCheatEnabled[CHEAT_GHOST_MODEL_SHOW]);
        }

        // CHEAT_VISIBLE_GHOST_AT_HUNT
        if (wParam == 0x37) {
            ApplicationInfo::bCheatEnabled[CHEAT_VISIBLE_GHOST_AT_HUNT] = !ApplicationInfo::bCheatEnabled[CHEAT_VISIBLE_GHOST_AT_HUNT];
            Gui::AddNotify("Visible Ghost Hunt", ApplicationInfo::bCheatEnabled[CHEAT_VISIBLE_GHOST_AT_HUNT] ? "Toggle: Enabled" : "Toggle: Disabled", 3.5f, COLOR_WHITE, COLOR_BLUE_HOVER);
        }

        break;
    case WM_CLOSE:
        if (MessageBox(nullptr, L"Exit?", L"Confirmation", MB_YESNO | MB_ICONQUESTION) == IDYES) {
            exit(0);
        }
        break;
    }

    if (ApplicationInfo::bMenuActive) {
        ClipCursor(nullptr);
        return ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
    }

    ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
    return 1;
}

void Gui::AddNotify(const std::string& title, const std::string& message, float displayTime, ImColor textColor, ImColor bgColor) {
    notificationManager.addNotification(title, message, displayTime, 10.0f, textColor, bgColor);
}

std::chrono::steady_clock::time_point lastFrameTime;
void Gui::updateLoop(std::chrono::steady_clock::time_point time) {
    lastFrameTime = time;
}

void DrawBoneConnections(II::Animator* pAnimator, float distance) {
    if (!pAnimator) return;

    auto pMainCamera = II::Camera::GetMain();
    if (!pMainCamera || bonePairs.empty()) return;

    for (const auto& [boneStart, boneEnd, label] : bonePairs) {
        auto pStart = pAnimator->GetBoneTransform(boneStart);
        auto pEnd = pAnimator->GetBoneTransform(boneEnd);

        if (pStart && pEnd) {
            auto pointStart = pMainCamera->WorldToScreenPoint(pStart->GetPosition(), UnityResolve::UnityType::Camera::Eye::Mono);
            auto pointEnd = pMainCamera->WorldToScreenPoint(pEnd->GetPosition(), UnityResolve::UnityType::Camera::Eye::Mono);

            if (pointStart.z > 0 && pointEnd.z > 0) {
                pointStart.y = ApplicationInfo::screenHeight - pointStart.y;
                pointEnd.y = ApplicationInfo::screenHeight - pointEnd.y;
                ImGui::GetBackgroundDrawList()->AddLine({ pointStart.x, pointStart.y }, { pointEnd.x, pointEnd.y }, ImColor(255, 255, 255, 255), 1.5f);
            }
        }
    }
}

auto CalculatePlayerData(Player* p) -> std::tuple<II::Vector3, II::Vector3, float, float> {
    if (!p) return {};

    auto playerPos = p->GetTransform()->GetPosition();
    auto camera = II::Camera::GetMain();
    if (!camera) return {};

    auto cameraPos = camera->GetTransform()->GetPosition();
    float distance = (playerPos - cameraPos).Length();
    float scale = p->GetTransform()->GetLocalScale().Length();

    return { playerPos, cameraPos, distance - scale, scale * 100.0f };
}

auto CalculateGhostData() -> std::tuple<II::Vector3, II::Vector3, float, float> {
    auto ghost = Ghost::gCurrentGhost;
    if (!ghost) return {};

    auto ghostPos = ghost->GetTransform()->GetPosition();
    auto camera = II::Camera::GetMain();
    if (!camera) return {};

    auto cameraPos = camera->GetTransform()->GetPosition();
    float distance = (ghostPos - cameraPos).Length();
    float scale = ghost->GetTransform()->GetLocalScale().Length();

    return { ghostPos, cameraPos, distance - scale, scale * 100.0f };
}

void ManageEMFList() 
{
    std::vector<EMF*> toRemove{};
    if (!Ghost::gCurrentGhost) {
        return;
    }

    if (!ApplicationInfo::bCheatEnabled[CHEAT_EMF_DATA]) {
        return;
    }

    if (!Ghost::emfData.empty()) {
        auto now = std::chrono::steady_clock::now();

        for (const auto& [emf, timestamp] : Ghost::emfData) {
            if (emf) {
                auto seconds_passed = std::chrono::duration_cast<std::chrono::seconds>(now - timestamp).count();

                FOR_EACH_COMPONENT(emf, II::Transform, "UnityEngine.CoreModule.dll", "Transform", pTransform) {
                    if (pTransform) {
                        const char* str = std::format("EMF ({})", 20 - seconds_passed).c_str();
                        DrawTextFromPosition(str, pTransform->GetPosition());
                    }
                }

                if (seconds_passed >= 20) {
                    toRemove.push_back(emf);
                }
            }
        }

        for (EMF* emf : toRemove) {
            if (emf) {
                Ghost::emfData.erase(
                    std::remove_if(Ghost::emfData.begin(), Ghost::emfData.end(),
                        [emf](const auto& tuple) {
                            return std::get<0>(tuple) == emf;
                        }), Ghost::emfData.end());
            }
        }

        toRemove.clear();
    }
}
extern DNAEvidence* pBone;
void ManageBoneEv() {
    if (Ghost::gCurrentGhost && pBone) 
    {
        auto pBoneObj = pBone->GetGameObject();
        if (pBoneObj) {
            auto pBoneTransform = pBoneObj->GetTransform();
            if (pBoneTransform) {
                DrawTextFromPosition("Bone", pBoneTransform->GetPosition());
            }
        }
    }
}

void Gui::DoDrawFeatures()
{
    auto currentFrameTime = std::chrono::high_resolution_clock::now();
    float deltaTime = std::chrono::duration<float>(currentFrameTime - lastFrameTime).count();
    lastFrameTime = currentFrameTime;

    ImGui::GetIO().DeltaTime = deltaTime;

    static bool bInj = false;

    if (!bInj) {
        Gui::AddNotify("Phasmohook", "Successfully injected!\n\nPress 'Enter' key for Open Menu!", 3.5f, COLOR_WHITE, COLOR_DARK_BLUE);
        bInj = true;
    }

    // Hunt notifications

    if (ApplicationInfo::bCheatEnabled[CHEAT_HUNT_NOTIFIES]) {
        static bool bHunting = false;

        if (Ghost::IsHunting() && !bHunting) {
            Gui::AddNotify("[!] HUNTING START", "The ghost has started the hunt", 3.5f, COLOR_WHITE, COLOR_RED);
            bHunting = true;
        }

        if (!Ghost::IsHunting() && bHunting) {
            if (ApplicationInfo::bCheatEnabled[CHEAT_VISIBLE_GHOST_AT_HUNT]) {
                ShowGhostModelForce(false); // fix visible ghost after hunt over
            }
            Gui::AddNotify("[!] HUNTING END", "The ghost stopped the hunt", 3.5f, COLOR_WHITE, COLOR_BLUE_HOVER);
            bHunting = false;
        }
    }

    notificationManager.render();

#ifdef __USE__PLAYER_POOLS
    if (!ApplicationInfo::bIsInLobby && ApplicationInfo::bCheatEnabled[CHEAT_PLAYERS_WALLHACK] && II::Camera::GetMain()) {
        try {
            for (int i = 0; i < MAX_PLAYERS; i++)
            {
                auto pPlayer = pPlayerPool->GetPlayer(i);
                if (pPlayer) {
                    auto playerObject = pPlayer->GetPlayer();
                    auto playerName = pPlayer->GetName();

                    if (playerObject && playerName)
                    {
                        if (!pPlayer->IsLocalPlayer() && Players::IsValidData(playerObject))
                        {
                            auto transform = playerObject->GetTransform();

                            if (transform) {
                                auto position = transform->GetPosition();

                                Log(std::format("Drawing text for player: {}", playerName));
                                DrawTextFromPosition(playerName, position);
                            }
                            else {
                                Log(std::format("Player {} has invalid transform.", playerName));
                            }
                        }
                    }
                    else {
                        Log(std::format("Player object or name is invalid for index: {}", i));
                    }
                }
            }
        }
        catch (const std::exception& e) {
            Log(std::format("Exception in wallhack drawing loop: {}", e.what()));
        }
        catch (...) {
            Log("Unknown exception in wallhack drawing loop.");
        }
    }
#endif

    //if (!ApplicationInfo::bIsInLobby) {
        /*try {
            if (Players::GetOnlinePlayersCount() > 0 && ApplicationInfo::bCheatEnabled[CHEAT_PLAYERS_WALLHACK]) {
                for (int i = 0; i < Players::GetOnlinePlayersCount(); i++)
                {
                    Player* pPlayer = Players::GetPlayerById(i);
                    if (Players::IsValidPlayer(pPlayer) && !Players::IsLocalPlayer(pPlayer))
                    {
                        auto [playerPos, playerCameraPos, playerDistance, playerBoxSizeMax] = CalculatePlayerData(pPlayer);
                        FOR_EACH_COMPONENT(pPlayer, II::Animator, "UnityEngine.AnimationModule.dll", "Animator", pAnimator) {
                            if (pAnimator) {
                                DrawBoneConnections(pAnimator, playerDistance);
                            }
                        }
                    }
                }
            }
        }
        catch (const std::exception& e) {
            Log(std::format("(Gui::DoDrawFeatures) Error: %s", e.what()));
        }
        catch (...) {
            Log(std::format("(Gui::DoDrawFeatures) Error: %s", "Unknown"));
        }*/
   // }
    

    ManageEMFList();
    ManageBoneEv();

    if (!ApplicationInfo::bIsInLobby && Ghost::gCurrentGhost && ApplicationInfo::bCheatEnabled[CHEAT_GHOST_WALLHACK]) {
        auto [ghostPos, cameraPos, distance, boxSizeMax] = CalculateGhostData();
        FOR_EACH_COMPONENT(Ghost::gCurrentGhost, II::Animator, "UnityEngine.AnimationModule.dll", "Animator", pAnimator) {
            if (pAnimator) {
                DrawBoneConnections(pAnimator, distance);
            }
        }
    }

    if (Ghost::gCurrentGhost && ApplicationInfo::bCheatEnabled[CHEAT_GHOST_INFO_SHOW])
    {
        if (Ghost::GetInfo(Ghost::gCurrentGhost) && !ApplicationInfo::bIsInLobby)
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

            ImGui::SetNextWindowSize(ImVec2(256, 256), ImGuiCond_Once);

            ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground;

            ImGui::Begin("#ghostInformation", nullptr, windowFlags);
           
            ImVec2 pos = ImGui::GetCursorScreenPos();

            DrawOutlinedText("Ghost type: ", Ghost::GetTypeName(), pos);

            pos.y += ImGui::GetTextLineHeight();

            DrawOutlinedText("Ghost state: ", Ghost::GetStateName(), pos);

            pos.y += ImGui::GetTextLineHeight();

            LevelRoom* pRoom = GetGhostFavouriteRoom();
            DrawOutlinedText("Ghost room: ", pRoom ? Room::GetRoomName(pRoom) : "", pos);

            pos.y += ImGui::GetTextLineHeight();

            DrawOutlinedText("Temperature: ", pRoom ? std::format("{}", Room::GetRoomTemperature(pRoom)).c_str() : "None", pos);

            pos.y += ImGui::GetTextLineHeight();

            DrawOutlinedText("Ghost name: ", Ghost::GetName(), pos);

            ImGui::End();
        }
    }
}

void Gui::RenderSideBar() {
    static float buttonHeight = 45.0f;
    
    auto createButton = [&](const char* label, int page) {
        if (selectedPage == page) {
            ImGui::PushStyleColor(ImGuiCol_Button, COLOR_RED);
        }

        if (ImGui::Button(label, ImVec2(-1, buttonHeight))) {
            selectedPage = page;
        }

        if (selectedPage == page)
            ImGui::PopStyleColor(); // Restore color
        };

    // Create buttons using the lambda
    createButton("Player", 0);
    createButton("Ghost", 1);
    createButton("Preferences", 2);
}

void Gui::RenderMainContent() 
{
    switch (selectedPage) {
        default: {
            ImGui::TextColored(ImVec4(0.7f, 0.9f, 0.7f, 1.0f), "About");
            ImGui::Separator();
            ImGui::Text("PHASMOHOOK - RELEASE - 1.0.1 - BLACK\nThanks for download\nBy 0xcds4r");
            break;
        }

        case 0: {
            ImGui::TextColored(ImVec4(0.7f, 0.9f, 0.7f, 1.0f), "Game Player / Cheats");
            ImGui::Separator();

            if (ImGui::Checkbox("Disable Stamina", &ApplicationInfo::bCheatEnabled[CHEAT_PLAYER_ANTISTAMINA])) {
                Gui::AddNotify("Disable Stamina", ApplicationInfo::bCheatEnabled[CHEAT_PLAYER_ANTISTAMINA] ? "Toggle: Enabled" : "Toggle: Disabled", 3.5f, COLOR_WHITE, COLOR_BLUE_HOVER);
            }

            /*if (ImGui::Checkbox("Players Wallhack", &ApplicationInfo::bCheatEnabled[CHEAT_PLAYERS_WALLHACK])) {
                Gui::AddNotify("Players Wallhack", ApplicationInfo::bCheatEnabled[CHEAT_PLAYERS_WALLHACK] ? "Toggle: Enabled" : "Toggle: Disabled", 3.5f, COLOR_WHITE, COLOR_BLUE_HOVER);
            }*/

#ifdef __USE__PLAYER_POOLS
            ImGui::TextColored(ImVec4(0.7f, 0.9f, 0.7f, 1.0f), "Online Players");
            ImGui::Separator();

            try {
                for (int i = 0; i < MAX_PLAYERS; i++)
                {
                    auto pPlayer = pPlayerPool->GetPlayer(i);
                    if (pPlayer) {
                        if (pPlayer->GetPlayer() && pPlayer->GetName() && Players::IsValidData(pPlayer->GetPlayer()))
                        {
                            ImGui::Text("[%d] %s %s", pPlayerPool->GetID(pPlayer->GetPlayer()), pPlayer->GetName(), pPlayer->IsLocalPlayer() ? "(YOU)" : "");
                        }
                    }
                }
            }
            catch (const std::exception& e) {
                ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Error: %s", e.what());
            }
            catch (...) {
                ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Unknown error occurred");
            }
#endif
            break;
        }


        case 1: {

            ImGui::TextColored(ImVec4(0.7f, 0.9f, 0.7f, 1.0f), "Ghost / Cheats");
            ImGui::Separator();

            if (ImGui::Checkbox("Ghost Wallhack", &ApplicationInfo::bCheatEnabled[CHEAT_GHOST_WALLHACK])) {
                Gui::AddNotify("Ghost Wallhack", ApplicationInfo::bCheatEnabled[CHEAT_GHOST_WALLHACK] ? "Toggle: Enabled" : "Toggle: Disabled", 3.5f, COLOR_WHITE, COLOR_BLUE_HOVER);
            }

            if (ImGui::Checkbox("Hunt notifications", &ApplicationInfo::bCheatEnabled[CHEAT_HUNT_NOTIFIES])) {
                Gui::AddNotify("Hunt notifications", ApplicationInfo::bCheatEnabled[CHEAT_HUNT_NOTIFIES] ? "Toggle: Enabled" : "Toggle: Disabled", 3.5f, COLOR_WHITE, COLOR_BLUE_HOVER);
            }

            if (ImGui::Checkbox("Ghost info show", &ApplicationInfo::bCheatEnabled[CHEAT_GHOST_INFO_SHOW])) {
                Gui::AddNotify("Ghost info show", ApplicationInfo::bCheatEnabled[CHEAT_GHOST_INFO_SHOW] ? "Toggle: Enabled" : "Toggle: Disabled", 3.5f, COLOR_WHITE, COLOR_BLUE_HOVER);
            }

            if (ImGui::Checkbox("EMF Show", &ApplicationInfo::bCheatEnabled[CHEAT_EMF_DATA])) {
                Gui::AddNotify("EMF Show", ApplicationInfo::bCheatEnabled[CHEAT_EMF_DATA] ? "Toggle: Enabled" : "Toggle: Disabled", 3.5f, COLOR_WHITE, COLOR_BLUE_HOVER);
            }

            if (ImGui::Checkbox("Ghost Model Show", &ApplicationInfo::bCheatEnabled[CHEAT_GHOST_MODEL_SHOW])) {
                Gui::AddNotify("Ghost Model Show", ApplicationInfo::bCheatEnabled[CHEAT_GHOST_MODEL_SHOW] ? "Toggle: Enabled" : "Toggle: Disabled", 3.5f, COLOR_WHITE, COLOR_BLUE_HOVER);
                ShowGhostModelForce(ApplicationInfo::bCheatEnabled[CHEAT_GHOST_MODEL_SHOW]);
            }

            if (ImGui::Checkbox("Visible Ghost Hunt", &ApplicationInfo::bCheatEnabled[CHEAT_VISIBLE_GHOST_AT_HUNT])) {
                Gui::AddNotify("Visible Ghost Hunt", ApplicationInfo::bCheatEnabled[CHEAT_VISIBLE_GHOST_AT_HUNT] ? "Toggle: Enabled" : "Toggle: Disabled", 3.5f, COLOR_WHITE, COLOR_BLUE_HOVER);
            }

            ImGui::TextColored(ImVec4(0.7f, 0.9f, 0.7f, 1.0f), "Ghost / Information");
            ImGui::Separator();

            if (ApplicationInfo::bIsInLobby) {
                ImGui::Text("You must be in game for see it :)");
            }
            else 
            {
                ImGui::Text("Name: %s", Ghost::GetName());
                ImGui::Text("Type: %s", Ghost::GetTypeName());
                ImGui::Text("State: %s", Ghost::GetStateName());
                ImGui::Text("Gender: %s", Ghost::GetSex());
                ImGui::Text("Age: %d", Ghost::GetAge());
                
                LevelRoom* pRoom = GetGhostFavouriteRoom();
                ImGui::Text("Fav. Room: %s", pRoom ? Room::GetRoomName(pRoom) : "");
            }
            
            break;
        }

        case 2: {
            ImGui::TextColored(ImVec4(0.7f, 0.9f, 0.7f, 1.0f), "Preferences");
            ImGui::Separator();
            break;
        }
    }

   
}

void Gui::RenderMainWindow()
{
    if (!ApplicationInfo::bMenuActive) {
        return;
    }


    ImGuiIO& io = ImGui::GetIO();

    ImVec2 screenCenter(io.DisplaySize.x / 2.0f, io.DisplaySize.y / 2.0f);
    static ImVec2 windowSize(io.DisplaySize.x * 0.4925f, io.DisplaySize.y * 0.47f);

    static ImVec2 windowPos(screenCenter.x - windowSize.x / 2.0f, screenCenter.y - windowSize.y / 2.0f);

    ImGui::SetNextWindowSize(windowSize, ImGuiCond_Once);
    ImGui::SetNextWindowPos(windowPos, ImGuiCond_Once);
    
    if (ImGui::Begin("Phasmohook", NULL, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse)) 
    {
        ImGui::BeginChild("Sidebar", ImVec2(150, -1), true);
            RenderSideBar();
        ImGui::EndChild();

        ImGui::SameLine();

        ImGui::BeginChild("MainContent", ImVec2(-1, -1), true);
            RenderMainContent();
        ImGui::EndChild();

        ImGui::End();
    }
}