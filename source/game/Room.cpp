#include "../main.h"

LevelRoom* Room::getRoomById(int id)
{
    std::lock_guard lock(roomsMutex);
    if (gRoomList.empty()) {
        return nullptr;
    }

    int i = 0;
    for (auto* room : gRoomList) {
        if (room != nullptr && i == id) {
            return room;
        }
        i++;
    }
    return nullptr;
}

const char* Room::GetRoomName(LevelRoom* _this)
{
    if (!_this) {
        return "";
    }

    if (!_this->GetGameObject()) {
        return "";
    }

    if (!_this->GetGameObject()->GetTransform()) {
        return "";
    }

    if (!*(uintptr_t*)((uintptr_t)_this + 0x60)) {
        return "";
    }

    II::String* offsetStr = GetOffsetValue<II::String*>((void*)_this, 0x60, "Assembly-CSharp.dll", "LevelRoom");
    std::string result = std::format("{}", offsetStr->ToString());
    return result.c_str();
}

float Room::GetRoomTemperature(LevelRoom* _this)
{
    if (!_this) {
        return 0.0f;
    }

    if (!_this->GetGameObject()) {
        return 0.0f;
    }

    if (!_this->GetGameObject()->GetTransform()) {
        return 0.0f;
    }

    if (!*(uintptr_t*)((uintptr_t)_this + 0x60)) {
        return 0.0f;
    }

    return *(float*)((uintptr_t)_this + 0x68);
}


const char* Room::GetRoomNameById(int id) {
    LevelRoom* pRoom = Room::getRoomById(id);
    if (pRoom != nullptr) {
        return Room::GetRoomName(pRoom);
    }

    return nullptr;
}

void Room::Init()
{
    //Log("Room::Init");

    Room::Reset();

    //setupHook("Assembly-CSharp.dll", "LevelRoom", "Awake", HAwake);
    //setupHook("Assembly-CSharp.dll", "LevelRoom", "Update", HUpdate);
}

void Room::Reset() {
    //Log("Room::Reset");
    //std::lock_guard lock(roomsMutex);
    //gRoomList.clear();
}

inline auto UNITY_CALLING_CONVENTION Room::HAwake(LevelRoom* _this) -> void
{
    //Log("Room::HAwake");

    //std::lock_guard lock(roomsMutex);
    H::Fcall(HAwake, _this);

    //gRoomList.push_back(_this);

    //LOGD("Room::HAwake >>");
    //LOGD(Room::GetRoomName(_this));
}

inline auto UNITY_CALLING_CONVENTION Room::HUpdate(LevelRoom* _this) -> void
{
    //Log("Room::HUpdate");
    //H::Fcall(HUpdate, _this);
    /* do something.. */
}