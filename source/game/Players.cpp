#include "../main.h"

extern PlayerPool* pPlayerPool;

void Players::Init()
{
	//Log("Players::Init");

	Players::Reset();

	//setupHook("Assembly-CSharp.dll", "Player", "Awake", HAwake);
	//setupHook("Assembly-CSharp.dll", "Player", "Update", HUpdate);
	//setupHook("Assembly-CSharp.dll", "Player", "OnDestroy", HOnDestroy);
}

void Players::Reset()
{
	//Log("Players::Reset");
	
#ifdef __USE__PLAYER_POOLS
	if (pPlayerPool) {
		for (int i = 0; i < MAX_PLAYERS; i++) {
			auto pPlayer = pPlayerPool->GetPlayer(i);
			if (pPlayer) {
				pPlayerPool->UnregisterPlayer(pPlayer->GetPlayer());
			}
		}
	}
#endif

	//PlayerPool::localPlayer = nullptr;
}

bool Players::IsLocalPlayer(Player* player)
{
	void* photonView = GetOffsetValue<void*>((void*)player, 0x20, "Assembly-CSharp.dll", "Player");

	if (!photonView) {
		return false;
	}

	void* owner = GetOffsetValue<void*>((void*)photonView, 0x80, "PhotonUnityNetworking.dll", "PhotonView");
	if (!owner) {
		return false;
	}

	return GetOffsetValue<bool>((void*)owner, 0x1C, "PhotonRealtime.dll", "Player");
}

Player* Players::GetLocalPlayer()
{
	return nullptr;
	//return PlayerPool::localPlayer;
}

const char* Players::GetUsername(Player* player)
{
	void* photonView = GetOffsetValue<void*>((void*)player, 0x20, "Assembly-CSharp.dll", "Player");

	if (!photonView) {
		return nullptr;
	}

	void* owner = GetOffsetValue<void*>((void*)photonView, 0x80, "PhotonUnityNetworking.dll", "PhotonView");
	if (!owner) {
		return nullptr;
	}

	return std::format("{}", GetOffsetValue<II::String*>((void*)owner, 0x20, "PhotonRealtime.dll", "Player")->ToString()).c_str();
}

bool Players::IsValidData(Player* player) {
	void* photonView = GetOffsetValue<void*>((void*)player, 0x20, "Assembly-CSharp.dll", "Player");

	if (!photonView) {
		return false;
	}

	void* owner = GetOffsetValue<void*>((void*)photonView, 0x80, "PhotonUnityNetworking.dll", "PhotonView");
	if (!owner) {
		return false;
	}

	return true;
}

const char* Players::GetCurrentRoomName(Player* player) {
	if (!player) {
		return "";
	}

	LevelRoom* pRoom = GetOffsetValue<LevelRoom*>((void*)player, 0x60, "Assembly-CSharp.dll", "Player");
	if (pRoom) {
		return Room::GetRoomName(pRoom);
	}
	return "";
}

inline auto UNITY_CALLING_CONVENTION Players::HAwake(Player* _this) -> void
{
	Log("Players::HAwake");
	
	H::Fcall(HAwake, _this);

	/*if (Players::IsValidData(_this))
	{
		if (Players::IsLocalPlayer(_this)) {
			PlayerPool::localPlayer = _this;
		}

		if (PlayerPool::contains(_this)) {
			PlayerPool::remove(_this);
		}

		PlayerPool::add(_this, Players::GetUsername(_this));
		
		if (GetLocalPlayer() == _this) {
			Gui::AddNotify("Lobby - Join", std::format("[YOU] {}", PlayerPool::get_name(_this)), 3.5f, COLOR_WHITE, COLOR_BLUE_HOVER);
		}
		else {
			Gui::AddNotify("Lobby - Join", std::format("{}", PlayerPool::get_name(_this)), 3.5f, COLOR_WHITE, COLOR_BLUE_HOVER);
		}
	}*/
	
	/*std::lock_guard lock(playersMutex);

	

	if (Players::IsValidPlayer(_this))
	{
		gPlayersList.push_back(_this);

		if (Players::IsLocalPlayer(_this)) {
			gLocalPlayer = _this;
			LOGD(std::format("[Lobby / Join / LocalPlayer] (ID: {}) Username: {}", Players::GetPlayerId(_this), Players::GetUsername(_this)));
			Gui::AddNotify("Lobby - Join", std::format("[YOU] [{}] {}", Players::GetPlayerId(_this), Players::GetUsername(_this)), 3.5f, COLOR_WHITE, COLOR_BLUE_HOVER);
		}
		else {
			Gui::AddNotify("Lobby - Join", std::format("[{}] {}", Players::GetPlayerId(_this), Players::GetUsername(_this)), 3.5f, COLOR_WHITE, COLOR_BLUE_HOVER);
			LOGD(std::format("[Lobby / Join / RemotePlayer] (ID: {}) Username: {}", Players::GetPlayerId(_this), Players::GetUsername(_this)));
		}
	}*/

#ifdef __USE__PLAYER_POOLS
	if (pPlayerPool && Players::IsValidData(_this)) {
		pPlayerPool->RegisterPlayer(_this);
	}
#endif
}

inline auto UNITY_CALLING_CONVENTION Players::HUpdate(Player* _this) -> void
{
	//Log("Players::HUpdate");

	H::Fcall(HUpdate, _this);
}

inline auto UNITY_CALLING_CONVENTION Players::HOnDestroy(Player* _this) -> void
{
	Log("Players::HOnDestroy");
#ifdef __USE__PLAYER_POOLS
	if (pPlayerPool && Players::IsValidData(_this)) {
		pPlayerPool->UnregisterPlayer(_this);
	}

#endif

	/*if (Players::IsValidData(_this)) {
		

		if (PlayerPool::contains(_this)) {
			
			if (Players::GetLocalPlayer() == _this) {
				Gui::AddNotify("Lobby - Left", std::format("[YOU] {}", PlayerPool::get_name(_this)), 3.5f, COLOR_WHITE, COLOR_BLUE_HOVER);
				PlayerPool::localPlayer = nullptr;
			}
			else {
				Gui::AddNotify("Lobby - Left", std::format("{}", PlayerPool::get_name(_this)), 3.5f, COLOR_WHITE, COLOR_BLUE_HOVER);
			}

			PlayerPool::remove(_this);
		}
	}*/

	/*std::lock_guard lock(playersMutex);

	if (Players::IsValidPlayer(_this))
	{
		if (Players::IsLocalPlayer(_this)) {
			gLocalPlayer = nullptr;
			Gui::AddNotify("Lobby - Left", std::format("[YOU] [{}] {}", Players::GetPlayerId(_this), Players::GetUsername(_this)), 3.5f, COLOR_WHITE, COLOR_BLUE_HOVER);
			LOGD(std::format("[Lobby / Left / LocalPlayer] (ID: {}) Username: {}", Players::GetPlayerId(_this), Players::GetUsername(_this)));
		}
		else {
			Gui::AddNotify("Lobby - Left", std::format("[{}] {}", Players::GetPlayerId(_this), Players::GetUsername(_this)), 3.5f, COLOR_WHITE, COLOR_BLUE_HOVER);
			LOGD(std::format("[Lobby / Left / RemotePlayer] (ID: {}) Username: {}", Players::GetPlayerId(_this), Players::GetUsername(_this)));
		}
	}

	auto it = std::find(gPlayersList.begin(), gPlayersList.end(), _this);

	if (it != gPlayersList.end()) {
		gPlayersList.erase(it);
	}*/

	H::Fcall(HOnDestroy, _this);
}