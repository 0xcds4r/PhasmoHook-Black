#pragma once

class Room
{
public:
	static void Init();
	static void Reset();

public:
	static inline LevelRoom* gCurrentRoom = nullptr;
	inline static std::vector<LevelRoom*> gRoomList{};
	static LevelRoom* getRoomById(int id);
	static const char* GetRoomName(LevelRoom* _this);
	static float GetRoomTemperature(LevelRoom* _this);
	static const char* GetRoomNameById(int id);

private:
	inline static std::mutex           roomsMutex;

	inline static auto UNITY_CALLING_CONVENTION  HAwake(LevelRoom* _this) -> void;
	inline static auto UNITY_CALLING_CONVENTION  HUpdate(LevelRoom* _this) -> void;
};

