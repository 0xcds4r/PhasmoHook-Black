#include "main.h"

#pragma comment(lib, "DbgHelp.lib")
#include <dbghelp.h>

Gui gui;

char ProcessInput(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return gui.ProcessInput(hWnd, msg, wParam, lParam);
}

const char* getStoragePath() {
	const char* homeDir = std::getenv("USERPROFILE");
	const char* path = "\\PhasmoHook\\";
	std::string desktopPath = std::string(homeDir) + std::string(path);
	return desktopPath.c_str();
}

void UpdateResolutionScale()
{
	auto screenWidth = ApplicationInfo::screenWidth;
	auto screenHeight = ApplicationInfo::screenHeight;

	if (screenWidth <= 0.0f || screenHeight <= 0.0f)
	{
		return;
	}

	ApplicationInfo::screenScaleX = 1.0f / screenWidth;
	ApplicationInfo::screenScaleY = 1.0f / screenHeight;
}

void FeatureUpdateLoop()
{
	gui.updateLoop(std::chrono::high_resolution_clock::now());

	tagRECT rect;
	while (true) {
		GetClientRect(dx_hook::Hk11::GetHwnd(), &rect);
		ApplicationInfo::screenWidth = static_cast<float>(rect.right - rect.left);
		ApplicationInfo::screenHeight = static_cast<float>(rect.bottom - rect.top);
		UpdateResolutionScale();
		Sleep(100);
	}
}

bool InitializeImGui() {
	if (!ApplicationInfo::bGUIInited) 
	{
		Log("Phasmohook initializing gui..");

		UnityResolve::ThreadAttach();

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImPlot::CreateContext();

		auto& io = ImGui::GetIO();
		//const char* homeDir = std::getenv("USERPROFILE");

		io.IniFilename = nullptr;

		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad | ImGuiConfigFlags_NavEnableKeyboard;
		//io.Fonts->ConfigData.Data->FontDataOwnedByAtlas = false;
		io.Fonts->AddFontFromMemoryTTF(Font::getallfont(), 2362740, 15, nullptr, io.Fonts->GetGlyphRangesCyrillic());

		ImGui_ImplWin32_Init(dx_hook::Hk11::GetHwnd());
		ImGui_ImplDX11_Init(dx_hook::Hk11::GetDevice(), dx_hook::Hk11::GetContext());

		dx_hook::Hk11::SetWndProc([](HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> char {
			return ProcessInput(hWnd, msg, wParam, lParam);
		});

		gui.ApplyStyles();
		ApplicationInfo::bGUIInited = true;
	}
	return ApplicationInfo::bGUIInited;
}

void RenderImGui() {
	if (!InitializeImGui()) {
		return;
	}

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	gui.RenderMainWindow();
	gui.DoDrawFeatures();

	ImGui::EndFrame();
	ImGui::Render();
	dx_hook::Hk11::GetContext()->OMSetRenderTargets(1, dx_hook::Hk11::GetTargetView(), nullptr);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void InitConfig() 
{
	Log("Initializing PhasmoHook Configs..");

	std::string configPath = std::string(getStoragePath()) + std::string(CONFIG_FILE_NAME);
	
	Log("Storage path: " + std::string(getStoragePath()));

	nlohmann::json jsonObj;
	jsonObj["version"] = "1.0.0";
	jsonObj["version_code"] = 101;
	jsonObj["version_tag"] = "black";

	std::ofstream file(configPath);
	if (file.is_open()) {
		file << jsonObj.dump(4);
		file.close();
	}
}

void InjectHooks();
void InitPlugin() {
	Log("Initializing PhasmoHook by 0xcds4r..");
	
	InitConfig();

	Network::Init();

	UnityResolve::Init(GetModuleHandleA("GameAssembly.dll"), UnityResolve::Mode::Il2Cpp);
		
	InjectHooks();
}

std::wstring stringToWString(const std::string& str) {
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), NULL, 0);
	std::wstring wstrTo(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), &wstrTo[0], size_needed);
	return wstrTo;
}

void CreateTextDump(EXCEPTION_POINTERS* pep)
{
	// Open file to save the dump
	std::string configPath = std::string(getStoragePath()) + std::string("crashdump.txt");

	HANDLE hFile = CreateFile((LPCWSTR)stringToWString(configPath).c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile != INVALID_HANDLE_VALUE)
	{
		// Write the exception information to the file
		DWORD dwWritten;
		WriteFile(hFile, "Exception Information:\n", 20, &dwWritten, NULL);
		WriteFile(hFile, "-------------------------\n", 20, &dwWritten, NULL);
		WriteFile(hFile, "Exception Code: ", 15, &dwWritten, NULL);
		WriteFile(hFile, std::to_wstring(pep->ExceptionRecord->ExceptionCode).c_str(), 10, &dwWritten, NULL);
		WriteFile(hFile, "\n", 1, &dwWritten, NULL);

		// Write the stack trace to the file
		WriteFile(hFile, "Stack Trace:\n", 12, &dwWritten, NULL);
		WriteFile(hFile, "-------------\n", 12, &dwWritten, NULL);
		CONTEXT context;
		context.ContextFlags = CONTEXT_FULL;
		GetThreadContext(GetCurrentThread(), &context);
		for (int i = 0; i < 10; i++)
		{
			WriteFile(hFile, std::to_wstring(context.Rip).c_str(), 10, &dwWritten, NULL);
			WriteFile(hFile, "\n", 1, &dwWritten, NULL);
			context.Rip = *(DWORD64*)context.Rsp;
			context.Rsp += 8;
		}

		CloseHandle(hFile);
	}
}

void Log(const std::string& message)
{
	std::string logPath = std::string(getStoragePath()) + std::string("phasmohook.log");
	HANDLE hFile = CreateFile((LPCWSTR)stringToWString(logPath).c_str(), GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile != INVALID_HANDLE_VALUE)
	{
		SetFilePointer(hFile, 0, NULL, FILE_END);

		DWORD dwWritten;
		SYSTEMTIME st;
		GetSystemTime(&st);
		char buffer[2048 + 1];
		sprintf(buffer, "%04d-%02d-%02d %02d:%02d:%02d: %s\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, message.c_str());
		WriteFile(hFile, buffer, strlen(buffer), &dwWritten, NULL);
		std::cout << message.c_str() << std::endl;
		CloseHandle(hFile);
	}
}

void CreateMiniDump(EXCEPTION_POINTERS* pep)
{
	CreateTextDump(pep);
}

LONG WINAPI MyUnhandledExceptionFilter(EXCEPTION_POINTERS* pExceptionInfo)
{
	std::cout << "Unhandled exception occurred! Creating a minidump...\n";

	CreateMiniDump(pExceptionInfo);

	return EXCEPTION_EXECUTE_HANDLER;
}

auto APIENTRY DllMain(HMODULE hModule, const DWORD ul_reason_for_call, LPVOID lpReserved) -> BOOL 
{
	SetConsoleOutputCP(CP_UTF8);

	if (ul_reason_for_call == DLL_PROCESS_ATTACH) 
	{
		ApplicationInfo::hModule = hModule;
		//SetUnhandledExceptionFilter(MyUnhandledExceptionFilter);

		if (HMODULE phasm = GetModuleHandle(GAME_PROCESS_NAME)) {
			std::thread([hModule] {
				console::StartConsole(L"Console", false);
				
				InitPlugin();

				dx_hook::Hk11::Build([] { RenderImGui(); });
				std::thread(FeatureUpdateLoop).detach();
				}
			).detach();
		}
	}
	return TRUE;
}

auto CALLBACK HookCallBack(int Code, WPARAM wParam, LPARAM lParam) -> LRESULT {
	std::wstring file(255, '\0');
	GetModuleFileNameW(ApplicationInfo::hModule, file.data(), 255);
	LoadLibraryW(file.c_str());
	return CallNextHookEx(nullptr, Code, wParam, lParam);
}

auto GetIdByName(const std::wstring& name) -> DWORD {
	HANDLE hsnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hsnapshot == INVALID_HANDLE_VALUE) return -1;

	PROCESSENTRY32 processer;
	processer.dwSize = sizeof(PROCESSENTRY32);

	for (int flag = Process32First(hsnapshot, &processer); flag; flag = Process32Next(hsnapshot, &processer)) {
		if (processer.szExeFile == name) {
			CloseHandle(hsnapshot);
			return processer.th32ProcessID;
		}
	}

	CloseHandle(hsnapshot);
	return -2;
}



static DWORD idThread;

auto CALLBACK EnumFunc(HWND hWnd, LPARAM pid) -> BOOL {
	DWORD dwProcessId;
	if (GetWindowThreadProcessId(hWnd, &dwProcessId) && dwProcessId == pid && IsWindowVisible(hWnd)) {
		idThread = GetWindowThreadProcessId(hWnd, nullptr);
		return FALSE; // Stop enumeration
	}
	return TRUE; // Continue enumeration
}

extern "C" _declspec(dllexport) auto Inject() -> void {
	if (auto pid = GetIdByName(GAME_PROCESS_NAME); pid != -1) {
		EnumWindows(EnumFunc, pid);
		if (auto hook = SetWindowsHookExW(WH_GETMESSAGE, HookCallBack, ApplicationInfo::hModule, idThread)) {
			PostThreadMessageW(idThread, WM_NULL, 0, 0);
		}
	}
}