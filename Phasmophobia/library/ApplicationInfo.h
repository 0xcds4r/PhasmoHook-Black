#pragma once

enum ApplicationLocale {
    APP_LOCALE_ENG = 0,
    APP_LOCALE_RUS = 1
};

class ApplicationInfo {
public:
    inline static bool bGUIInited;
    inline static WNDPROC originalWndProc;
    inline static HMODULE hModule;
    inline static float screenWidth;
    inline static float screenHeight;
    inline static float screenScaleX;
    inline static float screenScaleY;
    inline static bool bMenuActive;
    inline static bool bIsInLobby;
    inline static std::string versionInfo;
    inline static ApplicationLocale locale = APP_LOCALE_ENG;
    inline static bool bCheatEnabled[40]{};
    inline static float fPlayerSpeed = 1.60f;
    inline static Config* globalConfig = nullptr;
    //inline static float fPlayerDefaultSpeed = 1.60f;

    static Config* GetGlobalConfig() {
        return globalConfig;
    }

    static std::string GetVersion() {
        return versionInfo;
    }

    static bool IsEngLocale() {
        return locale == APP_LOCALE_ENG;
    }

    static bool IsRusLocale() {
        return locale == APP_LOCALE_RUS;
    }
};