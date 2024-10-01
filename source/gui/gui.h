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
#define COLOR_DARK_RED            HexToRGBA("852019FF") 

class Gui {
public:
    Gui() = default;
    ~Gui() = default;

    void ApplyStyles();

    char ProcessInput(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

    void RenderMainWindow();

    static void AddNotify(const std::string& title, const std::string& message, float displayTime = 3.5f, ImColor textColor = ImColor(255, 255, 255, 255), ImColor bgColor = ImColor(40, 40, 40, 255));

    void updateLoop(std::chrono::steady_clock::time_point time);

    void DoDrawFeatures();

    void RenderSideBar();

    void RenderMainContent();
    static inline DNAEvidence* pBone = nullptr;
protected:
    uint8_t selectedPage = -1;
    
};