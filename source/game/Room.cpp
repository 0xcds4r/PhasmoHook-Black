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

    II::String* offsetStr = GetOffsetValue<II::String*>((void*)_this, 0x60, "Assembly-CSharp.dll", "LevelRoom");
    if (!offsetStr) {
        return "";
    }

    std::string roomName = offsetStr->ToString();
    if (roomName.empty()) {
        return "";
    }

    static std::string result;
    result = roomName;
    return result.c_str();
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
    Log("Room::Init");

    Room::Reset();

    setupHook("Assembly-CSharp.dll", "LevelRoom", "Awake", HAwake);
    setupHook("Assembly-CSharp.dll", "LevelRoom", "Update", HUpdate);
}

void Room::Reset() {
    Log("Room::Reset");
    std::lock_guard lock(roomsMutex);
    gRoomList.clear();
}

inline auto UNITY_CALLING_CONVENTION Room::HAwake(LevelRoom* _this) -> void
{
    Log("Room::HAwake");

    std::lock_guard lock(roomsMutex);
    H::Fcall(HAwake, _this);

    Players::Reset();

    gRoomList.push_back(_this);

    //LOGD("Room::HAwake >>");
    //LOGD(Room::GetRoomName(_this));
}

inline auto UNITY_CALLING_CONVENTION Room::HUpdate(LevelRoom* _this) -> void
{
    //Log("Room::HUpdate");
    H::Fcall(HUpdate, _this);
    /* do something.. */
}