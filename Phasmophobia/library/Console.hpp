#pragma once

#include <Windows.h>
#include <cstdio>
#include <string>
#include <format>
#include <cstdarg>
#include <locale>
#include <codecvt>
#include <filesystem>

#define LOGD(text) console::OutConsole(console::Debug, std::string(text) + std::string("\n"), __FILE__, __LINE__)
#define LOGI(text) console::OutConsole(console::Info, std::string(text) + std::string("\n"), __FILE__, __LINE__)
#define LOGW(text) console::OutConsole(console::Warning, std::string(text) + std::string("\n"), __FILE__, __LINE__)
#define LOGE(text) console::OutConsole(console::Error, std::string(text) + std::string("\n"), __FILE__, __LINE__)

//std::string FormatString(const char* format, ...) {
//    va_list args;
//    va_start(args, format);
//
//    va_list args_copy;
//    va_copy(args_copy, args);
//    int size = vsnprintf(nullptr, 0, format, args_copy) + 1;
//    va_end(args_copy);
//
//    std::unique_ptr<char[]> buffer(new char[size]);
//    vsnprintf(buffer.get(), size, format, args);
//    va_end(args);
//
//    return std::string(buffer.get());
//}

namespace console {
    enum OutType : short int {
        Info,
        Debug,
        Warning,
        Error
    };

    enum Color : short int {
        Black,
        Blue,
        Green,
        LightGreen,
        Red,
        Purple,
        Yellow,
        White,
        Grey,
        LightBlue,
        ThinGreen,
        LightLightGreen,
        LightRed,
        Lavender,
        CanaryYellow,
        BrightWhite
    };

    static std::wstring Utf8ToWide(const std::string& str) {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
        return converter.from_bytes(str);
    }

    static void WriteConsoleWide(HANDLE hConsole, const std::wstring& text) {
        WriteConsoleW(hConsole, text.c_str(), static_cast<DWORD>(text.length()), nullptr, nullptr);
    }

    static void OutConsole(const OutType type, const std::string& text, const std::string& file, int line) {
        const auto hWnd_ = GetStdHandle(STD_OUTPUT_HANDLE);

        std::wstring wtext = Utf8ToWide(text);
        std::wstring wfile = Utf8ToWide(file);
        std::wstring wfile_name = wfile.substr(wfile.find_last_of(L'\\') + 1);
        std::wstring wline = std::to_wstring(line);

        switch (type) {
        case Info:
            SetConsoleTextAttribute(hWnd_, BACKGROUND_INTENSITY | Green * 16);
            WriteConsoleWide(hWnd_, L" ");
            SetConsoleTextAttribute(hWnd_, BACKGROUND_INTENSITY | Black);
            SetConsoleTextAttribute(hWnd_, FOREGROUND_INTENSITY | White);
            WriteConsoleWide(hWnd_, L"(");
            SetConsoleTextAttribute(hWnd_, FOREGROUND_INTENSITY | Green);
            WriteConsoleWide(hWnd_, L"Info ");
            break;
        case Debug:
            SetConsoleTextAttribute(hWnd_, BACKGROUND_INTENSITY | LightRed * 16);
            WriteConsoleWide(hWnd_, L" ");
            SetConsoleTextAttribute(hWnd_, BACKGROUND_INTENSITY | Black);
            SetConsoleTextAttribute(hWnd_, FOREGROUND_INTENSITY | White);
            WriteConsoleWide(hWnd_, L"(");
            SetConsoleTextAttribute(hWnd_, FOREGROUND_INTENSITY | LightRed);
            WriteConsoleWide(hWnd_, L"Debug");
            break;
        case Warning:
            SetConsoleTextAttribute(hWnd_, BACKGROUND_INTENSITY | Yellow * 16);
            WriteConsoleWide(hWnd_, L" ");
            SetConsoleTextAttribute(hWnd_, BACKGROUND_INTENSITY | Black);
            SetConsoleTextAttribute(hWnd_, FOREGROUND_INTENSITY | White);
            WriteConsoleWide(hWnd_, L"(");
            SetConsoleTextAttribute(hWnd_, FOREGROUND_INTENSITY | Yellow);
            WriteConsoleWide(hWnd_, L"Warn ");
            break;
        case Error:
            SetConsoleTextAttribute(hWnd_, BACKGROUND_INTENSITY | Red * 16);
            WriteConsoleWide(hWnd_, L" ");
            SetConsoleTextAttribute(hWnd_, BACKGROUND_INTENSITY | Black);
            SetConsoleTextAttribute(hWnd_, FOREGROUND_INTENSITY | White);
            WriteConsoleWide(hWnd_, L"(");
            SetConsoleTextAttribute(hWnd_, FOREGROUND_INTENSITY | Red);
            WriteConsoleWide(hWnd_, L"Error");
            break;
        }
        SetConsoleTextAttribute(hWnd_, FOREGROUND_INTENSITY | White);
        WriteConsoleWide(hWnd_, L") (");
        SetConsoleTextAttribute(hWnd_, FOREGROUND_INTENSITY | Red);
        WriteConsoleWide(hWnd_, wfile_name + L":" + wline);
        SetConsoleTextAttribute(hWnd_, FOREGROUND_INTENSITY | White);
        WriteConsoleWide(hWnd_, L") -> " + wtext);

        auto logPath = getStorage() / "phasmohook.log";
        HANDLE hFile = CreateFileW(
            logPath.wstring().c_str(),
            FILE_APPEND_DATA,
            FILE_SHARE_READ,
            nullptr,
            OPEN_ALWAYS,
            FILE_ATTRIBUTE_NORMAL,
            nullptr
        );

        if (hFile == INVALID_HANDLE_VALUE) {
            return;
        }

        if (GetFileSize(hFile, nullptr) == 0) {
            const char bom[] = "\xEF\xBB\xBF";
            DWORD dwWritten;
            WriteFile(hFile, bom, sizeof(bom) - 1, &dwWritten, nullptr);
        }

        SYSTEMTIME st;
        GetSystemTime(&st);

        std::string logEntry = std::format("{:04d}-{:02d}-{:02d} {:02d}:{:02d}:{:02d} [{}] {}:{} {}\n",
            st.wYear, st.wMonth, st.wDay,
            st.wHour, st.wMinute, st.wSecond,
            type == Info ? "INFO" : type == Debug ? "DEBUG" : type == Warning ? "WARN" : "ERROR",
            file.substr(file.find_last_of('\\') + 1), line, text);

        DWORD dwWritten;
        WriteFile(hFile, logEntry.data(), logEntry.size(), &dwWritten, nullptr);
        CloseHandle(hFile);
    }

    static HWND StartConsole(const wchar_t* title, const bool close) {
        HWND hWnd_ = nullptr;
        AllocConsole();
        SetConsoleTitleW(title);

        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);

        while (nullptr == hWnd_) hWnd_ = GetConsoleWindow();
        const auto menu_ = GetSystemMenu(hWnd_, FALSE);
        if (!close) DeleteMenu(menu_, SC_CLOSE, MF_BYCOMMAND);
        SetWindowLong(hWnd_, GWL_STYLE, GetWindowLong(hWnd_, GWL_STYLE) & ~WS_MAXIMIZEBOX);
        SetWindowLong(hWnd_, GWL_STYLE, GetWindowLong(hWnd_, GWL_STYLE) & ~WS_THICKFRAME);

        FILE* fp;
        freopen_s(&fp, "CONOUT$", "w+", stdout);
        freopen_s(&fp, "CONOUT$", "w+", stderr);
        freopen_s(&fp, "CONIN$", "r+", stdin);

        std::locale::global(std::locale("en_US.UTF-8"));

        return hWnd_;
    }

    static void EndConsole() {
        fclose(stdout);
        fclose(stderr);
        fclose(stdin);
        FreeConsole();
    }
}