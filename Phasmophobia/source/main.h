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
#include <memory>
#include <filesystem>

#pragma comment(lib, "DbgHelp.lib")
#include <dbghelp.h>
#include <psapi.h>

#include "utils/storage.h"
#include "utils/utils.h"
#include "utils/debugger.h"

#include "../library/Console.hpp"
#include "../library/D3D11Hook.h"
#include "../library/UnityResolve.hpp"
#include "../library/json.hpp"
#include "../library/detours/HookManager.h"
#include "../library/magic_enum/magic_enum_all.hpp"
#include "../library/Aliases.h"
#include "../library/HookHelper.h"

#include "utils/config.h"

#include "../library/ApplicationInfo.h"

#include "cheat/CheatManager.h"
#include "game/Game.h"
#include "gui/gui.h"