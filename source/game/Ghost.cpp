#include "../main.h"
#include "Game.h"

void Ghost::Init()
{
    Log("Ghost::Init");

    setupHook("Assembly-CSharp.dll", "GhostAI", "Awake", HAwake);
    setupHook("Assembly-CSharp.dll", "GhostAI", "Update", HUpdate);
}

void Ghost::Reset() {
    Log("Ghost::Reset");
    
    gCurrentGhost = nullptr;
    Ghost::emfData.clear();
}

const char* GetGhostTypeString(GhostType type) {
    auto it = GhostTypeNames.find(type);
    return (it != GhostTypeNames.end()) ? it->second : "Unknown";
}

const char* GetGhostStateString(GhostState state) {
    auto it = GhostStateNames.find(state);
    return (it != GhostStateNames.end()) ? it->second : "Unknown";
}

const char* GetEMFGhostActionNames(EMFGhostActionEv action) {
    auto it = EMFGhostActionNames.find(action);
    return (it != EMFGhostActionNames.end()) ? it->second : "Unknown";
}

const char* GetEMFGhostActionTypeNames(EMFGhostActionType action) {
    auto it = EMFGhostActionTypeNames.find(action);
    return (it != EMFGhostActionTypeNames.end()) ? it->second : "Unknown";
}

GhostState Ghost::GetState(GhostAI* _this) {
    if (!_this) return GhostState::Idle;
    return GetOffsetValue<GhostState>((void*)_this, 0x28, "Assembly-CSharp.dll", "GhostAI");
}

void* Ghost::GetGhostModel(GhostAI* _this) {
    if (!_this) return NULL;
    return GetOffsetValue<void*>((void*)_this, 0x60, "Assembly-CSharp.dll", "GhostAI");
}

GhostInfo* Ghost::GetInfo(GhostAI* _this) {
    if (!_this) return nullptr;
    return GetOffsetValue<GhostInfo*>((void*)_this, 0x38, "Assembly-CSharp.dll", "GhostAI");
}

bool Ghost::GetHunting(GhostAI* _this) {
    if (!_this) return false;
    return GetOffsetValue<bool>((void*)_this, 0xE9, "Assembly-CSharp.dll", "GhostAI");
}

bool Ghost::IsHunting() {
    if (Ghost::gCurrentGhost) {
        return Ghost::GetHunting(Ghost::gCurrentGhost);
    }
    return false;
}

const char* Ghost::GetName()
{
    if (Ghost::gCurrentGhost) {
        auto pInfo = Ghost::GetInfo(Ghost::gCurrentGhost);
        if (pInfo && pInfo->traits.name) {
            return pInfo->traits.name->ToString().c_str();
        }
    }
    return "None";
}

const char* Ghost::GetTypeName()
{
    if (Ghost::gCurrentGhost) {
        auto pInfo = Ghost::GetInfo(Ghost::gCurrentGhost);
        if (pInfo) {
            auto type = static_cast<GhostType>(static_cast<int>(pInfo->traits.typeFirst) + 1);
            return GetGhostTypeString(type);
        }
    }
    return "None";
}

int Ghost::GetAge() {
    if (Ghost::gCurrentGhost) {
        auto pInfo = Ghost::GetInfo(Ghost::gCurrentGhost);
        if (pInfo) {
            return pInfo->traits.age;
        }
    }
    return -1;
}

const char* Ghost::GetSex() {
    if (Ghost::gCurrentGhost) {
        auto pInfo = Ghost::GetInfo(Ghost::gCurrentGhost);
        if (pInfo) {
            return pInfo->traits.isMale ? "Male" : "Female";
        }
    }
    return "None";
}

int Ghost::GetFavouriteRoomID() {
    if (Ghost::gCurrentGhost) {
        auto pInfo = Ghost::GetInfo(Ghost::gCurrentGhost);
        if (pInfo) {
            return pInfo->traits.favouriteRoomID;
        }
    }
    return -1;
}

const char* Ghost::GetStateName() {
    if (Ghost::gCurrentGhost) {
        auto state = Ghost::GetState(Ghost::gCurrentGhost);
        return GetGhostStateString(state);
    }
    return "None";
}

inline auto UNITY_CALLING_CONVENTION Ghost::HAwake(GhostAI* _this) -> void
{
    H::Fcall(HAwake, _this);
    Ghost::gCurrentGhost = _this;
    ApplicationInfo::bIsInLobby = false;
}

inline auto UNITY_CALLING_CONVENTION Ghost::HUpdate(GhostAI* _this) -> void
{
    H::Fcall(HUpdate, _this);
}