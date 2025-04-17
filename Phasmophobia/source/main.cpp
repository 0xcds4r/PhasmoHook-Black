#include "main.h"
#include "utils/Event.h"

Gui gui;

extern auto ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM) -> LRESULT;

bool initializeImGui(HWND hWnd) 
{
    if (ApplicationInfo::bGUIInited) {
        return true;
    }

    LOGD(ApplicationInfo::IsEngLocale() ? "Initializing GUI..." : "Инициализация интерфейса...");

    UnityResolve::ThreadAttach();

    IMGUI_CHECKVERSION();

    if (!ImGui::CreateContext()) {
        LOGD("Failed to create ImGui context");
        return false;
    }

    if (!ImPlot::CreateContext()) {
        LOGD("Failed to create ImPlot context");
        ImGui::DestroyContext();
        return false;
    }

    auto& io = ImGui::GetIO();
    io.IniFilename = nullptr;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad | ImGuiConfigFlags_NavEnableKeyboard;

    if (!io.Fonts->AddFontFromMemoryTTF(Font::getallfont(), 2362740, 15, nullptr,
        io.Fonts->GetGlyphRangesCyrillic())) 
    {
        LOGD("Failed to load font");
        ImPlot::DestroyContext();
        ImGui::DestroyContext();

        return false;
    }

    if (!ApplicationInfo::originalWndProc) 
    {
        ApplicationInfo::originalWndProc = reinterpret_cast<WNDPROC>(GetWindowLongPtr(hWnd, GWLP_WNDPROC));

        if (!ApplicationInfo::originalWndProc) 
        {
            LOGD("Failed to get original window procedure");

            ImPlot::DestroyContext();
            ImGui::DestroyContext();

            return false;
        }
    }

    if (!ImGui_ImplWin32_Init(hWnd) ||
        !ImGui_ImplDX11_Init(dx_hook::Hk11::GetDevice(), dx_hook::Hk11::GetContext())) 
    {
        LOGD("Failed to initialize ImGui Win32 or DX11");

        ImPlot::DestroyContext();
        ImGui::DestroyContext();

        return false;
    }

    dx_hook::Hk11::SetWndProc([](HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> char 
    {
        if (gui.ProcessInput(hWnd, msg, wParam, lParam) == 0) {
            return ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
        }

        return CallWindowProc(ApplicationInfo::originalWndProc, hWnd, msg, wParam, lParam);
    });

    gui.ApplyStyles();

    ApplicationInfo::bGUIInited = true;

    return true;
}

void initConfig() {
    auto configPath = getStorage() / "phasmohook-cfg.json";
    ApplicationInfo::globalConfig = new Config(configPath);

    if (ApplicationInfo::GetGlobalConfig()->getLocale() == std::string("rus")) 
    {
        ApplicationInfo::locale = ApplicationLocale::APP_LOCALE_RUS;

        LOGD("Инициализация конфигураций..");
        LOGD("Локализация: Русская");
    }
    else 
    {
        ApplicationInfo::locale = ApplicationLocale::APP_LOCALE_ENG;

        LOGD("Initializing Configs..");
        LOGD("Locale: English");
    }

    ApplicationInfo::versionInfo = ApplicationInfo::GetGlobalConfig()->getVersion() + std::string("-") +
        ApplicationInfo::GetGlobalConfig()->getVersionTag() + std::string(" (build: ") +
        std::to_string(ApplicationInfo::GetGlobalConfig()->getVersionCode()) + std::string(")");
}

void setupDirectXHook() 
{
    dx_hook::Hk11::Build([]() 
    {
        HWND hWnd = dx_hook::Hk11::GetHwnd();
        if (!initializeImGui(hWnd)) {
            return;
        }

        RECT rect;
        if (GetClientRect(hWnd, &rect)) {
            ApplicationInfo::screenWidth = static_cast<float>(rect.right - rect.left);
            ApplicationInfo::screenHeight = static_cast<float>(rect.bottom - rect.top);
            ApplicationInfo::screenScaleX = 1.0f / ApplicationInfo::screenWidth;
            ApplicationInfo::screenScaleY = 1.0f / ApplicationInfo::screenHeight;
        }

        gui.UpdateDeltaTime();
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        gui.GlobalDraw();
        ImGui::EndFrame();
        ImGui::Render();

        auto device = dx_hook::Hk11::GetDevice();
        auto context = dx_hook::Hk11::GetContext();
        if (!device || !context) {
            LOGD("Error: Invalid DX11 device or context");
            return;
        }

        ID3D11RenderTargetView* const* targetPtr = dx_hook::Hk11::GetTargetView();
        context->OMSetRenderTargets(1, targetPtr, nullptr);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    });
}

void initializeCheat() 
{
    console::StartConsole(L"PhasmoHook", false);
    initConfig();

    LOGD(std::format("Initializing Hook v{} by 0xcds4r...", ApplicationInfo::versionInfo.c_str()));

    HMODULE gameAssembly = GetModuleHandleA("GameAssembly.dll");
    if (!gameAssembly) {
        LOGD("Failed to find GameAssembly.dll");
        return;
    }

    UnityResolve::Init(gameAssembly, UnityResolve::Mode::Il2Cpp);
    setupDirectXHook();

    Events::Initialise();
    CheatManager::Initialise();
}

void cleanup() 
{
    CheatManager::Uninitialise();
    Events::Uninitialise();

    if (ApplicationInfo::bGUIInited) 
    {
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImPlot::DestroyContext();
        ImGui::DestroyContext();
        ApplicationInfo::bGUIInited = false;
    }

    if (ApplicationInfo::globalConfig) {
        delete ApplicationInfo::globalConfig;
        ApplicationInfo::globalConfig = nullptr;
    }
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID) 
{
    if (reason == DLL_PROCESS_ATTACH) 
    {
        ApplicationInfo::hModule = hModule;
        if (!GetModuleHandleW(L"Phasmophobia.exe")) {
            return TRUE;
        }

        std::jthread([]() 
        {
            Sleep(3000); // disable steam-cache minidumps and etc shit..
            initializeCheat();
        }).detach();
	}
    else if (reason == DLL_PROCESS_DETACH) {
        cleanup();
    }

    return TRUE;
}