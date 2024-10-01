#pragma once

#include <fstream>
#include <Windows.h>
#include <future>
#include <thread>
#include <iostream>
#include <string>
#include <list>
#include <fstream>
#include <mutex>
#include <vector>
#include <cstdint>
#include <map>
#include <TlHelp32.h>
#include <ranges>
#include <wtypes.h>
#include <queue>
#include <typeindex>
#include <functional>

void Log(const std::string& message);

// log stuff
#include "../library/Console.hpp"

// d3d hook stuff
#include "../library/D3D11Hook.h"

// unity stuff
#include "../library/UnityResolve.hpp"

// other stuff
#include "../library/json.hpp"
#include "../library/detours/HookManager.h"
#include "../library/magic_enum/magic_enum_all.hpp"

#define GAME_PROCESS_NAME L"Phasmophobia.exe"
#define CONFIG_FILE_NAME "phasmohook-cfg.json"

#define U8(X) reinterpret_cast<const char*>(X)
using I = UnityResolve;
using IM = UnityResolve::Method;
using IC = UnityResolve::Class;
using IT = UnityResolve::Type;
using IF = UnityResolve::Field;
using IA = UnityResolve::Assembly;
using II = UnityResolve::UnityType;
using H = HookManager;

#define FOR_EACH_COMPONENT(gPtr, componentType, dllName, componentName, componentVar) \
    for (const auto componentVar : (gPtr)->GetGameObject()->GetComponentsInChildren<componentType*>(I::Get(dllName)->Get(componentName)))

#define GET_CLASS(dllName, className, classVar) \
    if (const auto classVar = I::Get(dllName)->Get(className))

class ApplicationInfo {
public:
    inline static bool bGUIInited;
    inline static HMODULE hModule;
    inline static float screenWidth;
    inline static float screenHeight;
    inline static float screenScaleX;
    inline static float screenScaleY;
    inline static bool bMenuActive;
	inline static bool bIsInLobby;
    inline static bool bCheatEnabled[16]{};
};

template <typename T>
T GetOffsetValue(void* _this, unsigned int offset, const char* library, const char* className)
{
	GET_CLASS(library, className, pClass) {
		try {
			return pClass->GetValue<T>(static_cast<void*>(_this), offset);
		}
		catch (const std::exception& ex) {
			LOGD(std::format("Error retrieving: {}", ex.what()));
		}
	}
	return T{};
}

template <typename HookFunc>
void setupHook(const char* dllName, const char* className, const char* methodName, HookFunc hookFunction) {
    GET_CLASS(dllName, className, pClass) {
        try {
            auto methodPointer = pClass->Get<IM>(methodName)->Cast<HookFunc>();
            if (methodPointer) {
                LOGD(std::format("Lib: {} | Class: {} | Method: {} -> success hooked!", dllName, className, methodName));
                H::InstallPtr(reinterpret_cast<void*>(methodPointer), reinterpret_cast<void*>(hookFunction));
            }
            else {
                LOGD(std::format("Error: method {} not found in class {}", methodName, className));
            }
        }
        catch (const std::exception& ex) {
            LOGD(std::format("Error setting up hook for {}::{}: {}", className, methodName, ex.what()));
        }
    }
}

template <typename HookFunc>
void setupRVAHook(void* funcAddress, HookFunc hookFunction) {
    try {
        auto methodPointer = II::AddrM(funcAddress);
        if (methodPointer) {
            LOGD("Method address resolved -> success hooked!");
            H::InstallPtr(methodPointer, reinterpret_cast<void*>(hookFunction));
        }
        else {
            LOGD("Error: method address could not be resolved.");
        }
    }
    catch (const std::exception& ex) {
        LOGD(std::format("Error setting up hook: {}", ex.what()));
    }
}

template<typename T>
void logOffsetValue(void* _this, unsigned int offset, const char* library, const char* className, const char* offsetName) {
    T value = GetOffsetValue<T>(_this, offset, library, className);

    if constexpr (std::is_same<T, bool>::value) {
        LOGD(std::format("{} 0x{:X}: {}", offsetName, offset, value ? "true" : "false"));
    }
    else if constexpr (std::is_integral<T>::value) {
        LOGD(std::format("{} 0x{:X}: {}", offsetName, offset, value));
    }
    else if constexpr (std::is_floating_point<T>::value) {
        LOGD(std::format("{} 0x{:X}: {:.2f}", offsetName, offset, value));
    }
    else if constexpr (std::is_same<T, std::string>::value) {
        LOGD(std::format("{} 0x{:X}: {}", offsetName, offset, value));
    }
    else {
        LOGD(std::format("{} 0x{:X}: Unknown type", offsetName, offset));
    }
}

#include "game/Game.h"
#include "networking/Network.h"
#include "gui/gui.h"