#include <windows.h>
#include <tlhelp32.h>
#include <string>

#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib") 

HWND hWndMain;
HWND hStatusLabel;
HWND hInjectButton;
HWND hAutoInjectCheckbox;
bool isRunning = false;
bool isInjected = false;
bool isAutoInject = true;
DWORD lastProcessId = 0;

DWORD GetProcessIdByName(const wchar_t* processName) {
    DWORD processId = 0;
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot != INVALID_HANDLE_VALUE) {
        PROCESSENTRY32W pe32 = { sizeof(PROCESSENTRY32W) };
        if (Process32FirstW(snapshot, &pe32)) {
            do {
                if (_wcsicmp(pe32.szExeFile, processName) == 0) {
                    processId = pe32.th32ProcessID;
                    break;
                }
            } while (Process32NextW(snapshot, &pe32));
        }
        CloseHandle(snapshot);
    }
    return processId;
}

bool InjectDLL(DWORD processId, const char* dllPath) {
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
    if (!hProcess) {
        MessageBoxA(NULL, "Failed to open process", "Error", MB_OK | MB_ICONERROR);
        return false;
    }

    LPVOID remoteMemory = VirtualAllocEx(hProcess, NULL, strlen(dllPath) + 1,
        MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!remoteMemory) {
        MessageBoxA(NULL, "Failed to allocate memory", "Error", MB_OK | MB_ICONERROR);
        CloseHandle(hProcess);
        return false;
    }

    if (!WriteProcessMemory(hProcess, remoteMemory, dllPath, strlen(dllPath) + 1, NULL)) {
        MessageBoxA(NULL, "Failed to write DLL path", "Error", MB_OK | MB_ICONERROR);
        VirtualFreeEx(hProcess, remoteMemory, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return false;
    }

    HMODULE hKernel32 = GetModuleHandleA("kernel32.dll");
    if (!hKernel32) {
        MessageBoxA(NULL, "Failed to get kernel32.dll", "Error", MB_OK | MB_ICONERROR);
        VirtualFreeEx(hProcess, remoteMemory, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return false;
    }

    LPVOID loadLibraryAddr = (LPVOID)GetProcAddress(hKernel32, "LoadLibraryA");
    if (!loadLibraryAddr) {
        MessageBoxA(NULL, "Failed to find LoadLibraryA", "Error", MB_OK | MB_ICONERROR);
        VirtualFreeEx(hProcess, remoteMemory, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return false;
    }

    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0,
        (LPTHREAD_START_ROUTINE)loadLibraryAddr, remoteMemory, 0, NULL);
    if (!hThread) {
        MessageBoxA(NULL, "Failed to create thread", "Error", MB_OK | MB_ICONERROR);
        VirtualFreeEx(hProcess, remoteMemory, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return false;
    }

    WaitForSingleObject(hThread, INFINITE);
    VirtualFreeEx(hProcess, remoteMemory, 0, MEM_RELEASE);
    CloseHandle(hThread);
    CloseHandle(hProcess);
    return true;
}

void PerformInjection() {
    const wchar_t* targetProcess = L"Phasmophobia.exe";
    char dllPath[MAX_PATH];
    GetModuleFileNameA(NULL, dllPath, MAX_PATH);
    std::string path(dllPath);
    path = path.substr(0, path.find_last_of("\\/")) + "\\PhasmoHook-Black.dll";

    if (!PathFileExistsA(path.c_str())) {
        SetWindowTextW(hStatusLabel, L"Status: PhasmoHook-Black.dll not found");
        isInjected = false;
        return;
    }

    DWORD processId = GetProcessIdByName(targetProcess);

    if (processId == 0) {
        SetWindowTextW(hStatusLabel, L"Status: Phasmophobia.exe not found");
        isInjected = false;
        lastProcessId = 0;
    }
    else if (processId != lastProcessId || !isInjected) {
        SetWindowTextW(hStatusLabel, L"Status: Injecting...");
        if (InjectDLL(processId, path.c_str())) {
            SetWindowTextW(hStatusLabel, L"Status: DLL successfully injected");
            isInjected = true;
            lastProcessId = processId;
        }
        else {
            SetWindowTextW(hStatusLabel, L"Status: Injection failed");
            isInjected = false;
        }
    }
}

void UpdateStatus() {
    const wchar_t* targetProcess = L"Phasmophobia.exe";
    DWORD processId = GetProcessIdByName(targetProcess);

    if (processId == 0) {
        SetWindowTextW(hStatusLabel, L"Status: Phasmophobia.exe not found");
        EnableWindow(hInjectButton, FALSE);
        isInjected = false;
        lastProcessId = 0;
    }
    else {
        EnableWindow(hInjectButton, TRUE);
        if (!isInjected) {
            SetWindowTextW(hStatusLabel, L"Status: Process found, ready to inject");
            if (isAutoInject && processId != lastProcessId) {
                PerformInjection();
            }
        }
    }
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    HFONT hFont = CreateFontW(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, L"Segoe UI");

    switch (msg) {
    case WM_CREATE:
        hStatusLabel = CreateWindowW(L"STATIC", L"Status: Waiting...",
            WS_CHILD | WS_VISIBLE | SS_CENTER, 10, 10, 380, 30, hwnd, NULL, NULL, NULL);
        SendMessage(hStatusLabel, WM_SETFONT, (WPARAM)hFont, TRUE);

        hInjectButton = CreateWindowW(L"BUTTON", L"Inject DLL",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 150, 50, 100, 30, hwnd, (HMENU)1, NULL, NULL);
        SendMessage(hInjectButton, WM_SETFONT, (WPARAM)hFont, TRUE);
        EnableWindow(hInjectButton, FALSE);

        hAutoInjectCheckbox = CreateWindowW(L"BUTTON", L"Auto Inject",
            WS_CHILD | WS_VISIBLE | BS_CHECKBOX, 10, 50, 100, 30, hwnd, (HMENU)2, NULL, NULL);
        SendMessage(hAutoInjectCheckbox, WM_SETFONT, (WPARAM)hFont, TRUE);
        SendMessage(hAutoInjectCheckbox, BM_SETCHECK, BST_CHECKED, 0);

        SetTimer(hwnd, 1, 1000, NULL);
        break;

    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case 1:
            PerformInjection();
            break;
        case 2: 
            if (SendMessage(hAutoInjectCheckbox, BM_GETCHECK, 0, 0) == BST_CHECKED) {
                SendMessage(hAutoInjectCheckbox, BM_SETCHECK, BST_UNCHECKED, 0);
                isAutoInject = false;
                SetWindowTextW(hStatusLabel, L"Status: Auto injection disabled");
            }
            else {
                SendMessage(hAutoInjectCheckbox, BM_SETCHECK, BST_CHECKED, 0);
                isAutoInject = true;
                SetWindowTextW(hStatusLabel, L"Status: Auto injection enabled");
            }
            break;
        }
        break;

    case WM_TIMER:
        if (isRunning) {
            UpdateStatus();
        }
        break;

    case WM_DESTROY:
        KillTimer(hwnd, 1);
        PostQuitMessage(0);
        break;

    case WM_CTLCOLORSTATIC:
        SetTextColor((HDC)wParam, RGB(0, 0, 0));
        SetBkColor((HDC)wParam, RGB(240, 240, 240));
        return (LRESULT)CreateSolidBrush(RGB(240, 240, 240));

    default:
        return DefWindowProcW(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
    WNDCLASSEXW wc = { 0 };
    wc.cbSize = sizeof(WNDCLASSEXW);
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"PhasmoInjectorClass";
    wc.hbrBackground = CreateSolidBrush(RGB(240, 240, 240));
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    if (!RegisterClassExW(&wc)) {
        return 1;
    }

    hWndMain = CreateWindowW(
        L"PhasmoInjectorClass",
        L"PhasmoHook Injector",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 120,
        NULL, NULL, hInstance, NULL
    );

    if (!hWndMain) {
        return 2;
    }

    isRunning = true;
    ShowWindow(hWndMain, nCmdShow);
    UpdateWindow(hWndMain);

    MSG msg;
    while (GetMessageW(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return (int)msg.wParam;
}