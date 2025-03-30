#include "main.h"
#include <memory>
#include <filesystem>

#pragma comment(lib, "DbgHelp.lib")
#include <dbghelp.h>

Gui gui;

char ProcessInput(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    return gui.ProcessInput(hWnd, msg, wParam, lParam);
}

class PhasmoHook {
private:
    static constexpr const char* CONFIG_FILE_NAME = "phasmohook-cfg.json";
    static constexpr const wchar_t* GAME_PROCESS_NAME = L"Phasmophobia.exe";

    mutable std::filesystem::path storagePath;

    static PhasmoHook* currentInstance;

    static char WndProcWrapper(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
        if (currentInstance) {
            return currentInstance->processGUIInput(hWnd, msg, wParam, lParam);
        }
        return 0;
    }

public:
    PhasmoHook() {
        SetConsoleOutputCP(CP_UTF8);
        SetUnhandledExceptionFilter(MyUnhandledExceptionFilter);
        currentInstance = this;
    }

    ~PhasmoHook() {
        if (currentInstance == this) {
            currentInstance = nullptr;
        }
    }

    std::filesystem::path getStoragePath() const {
        if (storagePath.empty()) {
            if (const char* homeDir = std::getenv("USERPROFILE")) {
                storagePath = std::filesystem::path(homeDir) / "PhasmoHook";
            }
        }
        return storagePath;
    }

    static void CreateMiniDump(EXCEPTION_POINTERS* pep, const std::filesystem::path& storagePath) {
        std::filesystem::path dumpPath = storagePath / "crashdump.txt";

        HANDLE hFile = CreateFileW(
            dumpPath.wstring().c_str(),
            GENERIC_WRITE,
            0,
            nullptr,
            CREATE_ALWAYS,
            FILE_ATTRIBUTE_NORMAL,
            nullptr
        );

        if (hFile != INVALID_HANDLE_VALUE) {
            DWORD dwWritten;
            std::string header = "Exception Information:\n-------------------------\n";
            WriteFile(hFile, header.c_str(), header.size(), &dwWritten, nullptr);

            std::string exceptionCode = "Exception Code: " + std::to_string(pep->ExceptionRecord->ExceptionCode) + "\n";
            WriteFile(hFile, exceptionCode.c_str(), exceptionCode.size(), &dwWritten, nullptr);

            CloseHandle(hFile);
        }
    }

    static LONG WINAPI MyUnhandledExceptionFilter(EXCEPTION_POINTERS* pExceptionInfo) {
        PhasmoHook phasmo;
        std::cout << "Unhandled exception occurred! Creating a minidump...\n";
        CreateMiniDump(pExceptionInfo, phasmo.getStoragePath());
        return EXCEPTION_EXECUTE_HANDLER;
    }

    char processGUIInput(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
        return gui.ProcessInput(hWnd, msg, wParam, lParam);
    }

    void updateResolutionScale(float width, float height) {
        if (width > 0.0f && height > 0.0f) {
            ApplicationInfo::screenWidth = width;
            ApplicationInfo::screenHeight = height;
            ApplicationInfo::screenScaleX = 1.0f / width;
            ApplicationInfo::screenScaleY = 1.0f / height;
        }
    }

    void featureUpdateLoop() {
        gui.updateLoop(std::chrono::high_resolution_clock::now());
        tagRECT rect;
        while (true) {
            if (GetClientRect(dx_hook::Hk11::GetHwnd(), &rect)) {
                updateResolutionScale(
                    static_cast<float>(rect.right - rect.left),
                    static_cast<float>(rect.bottom - rect.top)
                );
            }
            Sleep(100);
        }
    }


    bool initializeImGui() {
        if (ApplicationInfo::bGUIInited) return true;

        Log("Phasmohook initializing gui..");
        UnityResolve::ThreadAttach();

        IMGUI_CHECKVERSION();
        if (!ImGui::CreateContext() || !ImPlot::CreateContext()) {
            return false;
        }

        auto& io = ImGui::GetIO();
        io.IniFilename = nullptr;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad | ImGuiConfigFlags_NavEnableKeyboard;

        io.Fonts->AddFontFromMemoryTTF(Font::getallfont(), 2362740, 15, nullptr,
            io.Fonts->GetGlyphRangesCyrillic());

        if (!ImGui_ImplWin32_Init(dx_hook::Hk11::GetHwnd()) ||
            !ImGui_ImplDX11_Init(dx_hook::Hk11::GetDevice(), dx_hook::Hk11::GetContext())) {
            return false;
        }

        dx_hook::Hk11::SetWndProc([](HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> char {
            return ProcessInput(hWnd, msg, wParam, lParam);
        });

        gui.ApplyStyles();
        ApplicationInfo::bGUIInited = true;
        return true;
    }

    void initConfig() {
        Log("Initializing PhasmoHook Configs..");
        auto configPath = getStoragePath() / CONFIG_FILE_NAME;

        try {
            nlohmann::json jsonObj{
                {"version", "1.2"},
                {"version_code", 120},
                {"version_tag", "black"}
            };

            std::ofstream file(configPath, std::ios::out | std::ios::trunc);
            if (file.is_open()) {
                file << jsonObj.dump(4);
            }
        }
        catch (const std::exception& e) {
            Log(std::string("Config init failed: ") + e.what());
        }
    }

    void Log(const std::string_view message) {
        auto logPath = getStoragePath() / "phasmohook.log";

        HANDLE hFile = CreateFileW(
            logPath.wstring().c_str(),
            FILE_APPEND_DATA,
            FILE_SHARE_READ,
            nullptr,
            OPEN_ALWAYS,
            FILE_ATTRIBUTE_NORMAL,
            nullptr
        );

        if (hFile != INVALID_HANDLE_VALUE) {
            SYSTEMTIME st;
            GetSystemTime(&st);

            char buffer[2048];
            int len = sprintf_s(buffer, sizeof(buffer),
                "%04d-%02d-%02d %02d:%02d:%02d: %.*s\n",
                st.wYear, st.wMonth, st.wDay,
                st.wHour, st.wMinute, st.wSecond,
                static_cast<int>(message.size()), message.data()
            );

            DWORD dwWritten;
            WriteFile(hFile, buffer, len, &dwWritten, nullptr);
            CloseHandle(hFile);

            std::cout << message << std::endl;
        }
    }

    Gui& getGui() { return gui; }

    static const wchar_t* GetGameProcessName() { return GAME_PROCESS_NAME; }
};

PhasmoHook* PhasmoHook::currentInstance = nullptr;
PhasmoHook phasmo;

void InjectHooks();

void DebugClass(const char* dll, const char* className) {
    UnityResolve::Class* cls = UnityResolve::Get(dll)->Get(className);
    if (cls) {
        std::cout << "Fields in " << className << ":" << std::endl;
        for (const auto* field : cls->fields) {
            std::cout << "Field: " << field->name << ", Offset: 0x" << std::hex << field->offset << std::endl;
        }
        std::cout << "Methods in " << className << ":" << std::endl;
        for (const auto* method : cls->methods) {
            std::cout << "Method: " << method->name << std::endl;
        }
    }
    else {
        std::cout << "Failed to find " << className << std::endl;
    }
}

// universal
bool SetGamma(float gamma) {
    HDC hDC = GetDC(NULL); 
    if (!hDC) {
        std::cerr << "Failed to get device context" << std::endl;
        return false;
    }

    WORD gammaRamp[3][256];

    for (int i = 0; i < 256; i++) {
        float normalized = i / 255.0f;
        float adjusted = powf(normalized, 1.0f / gamma);
        WORD value = static_cast<WORD>(adjusted * 65535.0f);

        if (value > 65535) value = 65535;
        if (value < 0) value = 0;

        gammaRamp[0][i] = value;
        gammaRamp[1][i] = value;
        gammaRamp[2][i] = value; 
    }

    BOOL result = SetDeviceGammaRamp(hDC, gammaRamp);
    if (!result) {
        std::cerr << "Failed to set gamma ramp" << std::endl;
        ReleaseDC(NULL, hDC);
        return false;
    }

    ReleaseDC(NULL, hDC);
    return true;
}

void ResetGamma() {
    HDC hDC = GetDC(NULL);
    if (!hDC) {
        std::cerr << "Failed to get device context for reset" << std::endl;
        return;
    }

    WORD gammaRamp[3][256];
    for (int i = 0; i < 256; i++) {
        WORD value = i * 257;
        gammaRamp[0][i] = value;
        gammaRamp[1][i] = value;
        gammaRamp[2][i] = value;
    }

    if (!SetDeviceGammaRamp(hDC, gammaRamp)) {
        std::cerr << "Failed to reset gamma ramp" << std::endl;
    }

    ReleaseDC(NULL, hDC);
}

//void printLightNames() 
//{
//    auto* mainCamera = II::Camera::GetMain();
//    if (mainCamera) {
//        auto lights = II::Light::FindAll();
//        if (!lights.empty()) {
//            for (auto* light : lights) {
//                if (light) {
//                    auto* obj = light->GetGameObject();
//                    if (obj) {
//                        std::string stringObjName = std::format("{}", obj->GetName()->ToString());
//                        phasmo.Log(stringObjName);
//                    }
//                }
//            }
//        }
//    }
//}
//
//void printAllColliderNames() {
//    auto* mainCamera = II::Camera::GetMain();
//    if (!mainCamera) return;
//    auto colliders = UnityResolve::UnityType::Collider::FindAll();
//    if (!colliders.empty()) {
//        std::cout << "Colliders found in the scene:" << std::endl;
//        for (auto* collider : colliders) {
//            if (collider) {
//                auto* go = collider->GetGameObject();
//                if (go) {
//                    std::string stringObjName = std::format("{}", go->GetName()->ToString());
//                    std::cout << " - " << stringObjName << std::endl;
//                }
//                else {
//                    std::cout << " - (No GameObject attached)" << std::endl;
//                }
//            }
//        }
//    }
//    else {
//        //std::cerr << "No colliders found in the scene" << std::endl;
//    }
//}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID) {
    if (reason != DLL_PROCESS_ATTACH) return TRUE;

    //ResetGamma();
    
    ApplicationInfo::hModule = hModule;

    if (!GetModuleHandleW(PhasmoHook::GetGameProcessName())) {
        return TRUE;
    }

    std::thread([hModule, phasmo = std::move(phasmo)]() mutable {
        console::StartConsole(L"Console", false);

        phasmo.Log("Initializing PhasmoHook by 0xcds4r..");
        phasmo.initConfig();

        Network::Init();
        UnityResolve::Init(GetModuleHandleA("GameAssembly.dll"), UnityResolve::Mode::Il2Cpp);
        InjectHooks();

        dx_hook::Hk11::Build([&phasmo]() {
            if (phasmo.initializeImGui()) {
                ImGui_ImplDX11_NewFrame();
                ImGui_ImplWin32_NewFrame();
                ImGui::NewFrame();

                gui.GlobalDraw();

                ImGui::EndFrame();
                ImGui::Render();

                auto* context = dx_hook::Hk11::GetContext();
                ID3D11RenderTargetView* const* targetPtr = dx_hook::Hk11::GetTargetView();
                context->OMSetRenderTargets(1, targetPtr, nullptr); 
                ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
            }
            });

        std::thread([&phasmo]() { phasmo.featureUpdateLoop(); }).detach();
        }).detach();
    return TRUE;
}