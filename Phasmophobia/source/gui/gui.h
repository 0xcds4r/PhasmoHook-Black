#pragma once

#include "../../library/imgui-helper.h"

#include "NotificationWrapper.h"
ImVec4 HexToRGBA(const std::string& hex);

#define CHEAT_PLAYER_ANTISTAMINA 0
#define CHEAT_PLAYERS_WALLHACK 1
#define CHEAT_GHOST_WALLHACK 2
#define CHEAT_HUNT_NOTIFIES 3
#define CHEAT_GHOST_INFO_SHOW 4
#define CHEAT_EMF_DATA 5
#define CHEAT_GHOST_MODEL_SHOW 6
#define CHEAT_VISIBLE_GHOST_AT_HUNT 7
#define CHEAT_FULLBRIGHT 8
#define CHEAT_SUPERFLASHLIGHT 9
#define CHEAT_CURSED_ITEMS_SHOW 10
#define CHEAT_BONE_SHOW 11
#define CHEAT_NOCLIP 12
#define CHEAT_NOHUNTING 13
#define CHEAT_PLAYERSPEED 14

#define COLOR_WHITE          HexToRGBA("FFFFFFFF")
#define COLOR_DARK_BLUE      HexToRGBA("1A1A3DFF")  
#define COLOR_BLUE_HOVER     HexToRGBA("666699FF") 
#define COLOR_BLUE_ACTIVE    HexToRGBA("9999CCFF")  
#define COLOR_BLUE_UNFOCUS   HexToRGBA("333366FF")  
#define COLOR_BORDER         HexToRGBA("CCCCFFFF") 
#define COLOR_TEXT           HexToRGBA("E6E6E6FF") 
#define COLOR_TEXT_DISABLED  HexToRGBA("666666FF")  
#define COLOR_WINDOW_BG      HexToRGBA("0D0D33FF") 
#define COLOR_POPUP_BG       HexToRGBA("1A1A40FF") 
#define COLOR_RED            HexToRGBA("C43329FF") 


#define COLOR_RED_HOVER         ImVec4(0.90f, 0.10f, 0.10f, 1.00f)  
#define COLOR_RED_ACTIVE        ImVec4(0.70f, 0.00f, 0.00f, 1.00f)

#define COLOR_DARK_RED            HexToRGBA("852019FF") 

class Gui {
private:
    std::chrono::high_resolution_clock::time_point lastFrameTime = std::chrono::high_resolution_clock::now();
    std::atomic<float> currentDeltaTime{ 0.016f };
    int selectedPage = 0;


public:
    Gui() = default;
    ~Gui() = default;

    void ApplyStyles();
    void ManageNoclip();
    void ResetNoclip();

    char ProcessInput(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

    void GlobalDraw();

    static void AddNotify(const std::string& title, const std::string& message, float displayTime = 3.5f, ImColor textColor = ImColor(255, 255, 255, 255), ImColor bgColor = ImColor(40, 40, 40, 255));

    void updateLoop(std::chrono::high_resolution_clock::time_point time);
    void UpdateDeltaTime();
private:
    void ManageInjectNotifies();
    void DisplayGhostInfo();
    void ManageGhostWallhack();
    void ManageHuntNotifies();
    void ManagePlayersWallhack();
    void ProcessCheats();
    void RenderMainWindow();
    void DoDrawFeatures();
    void RenderSideBar();
    void RenderMainContent();
    void RenderPlayerPage(const ImVec4& titleColor);
    void RenderGhostPage(const ImVec4& titleColor);
    void RenderPreferencesPage(const ImVec4& titleColor);
    void RenderConsolePage(const ImVec4& titleColor);
};

void ResetAllGUI();