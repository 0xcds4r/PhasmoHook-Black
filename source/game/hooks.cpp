#include "../main.h"

LevelController* gLevelController = nullptr;
DNAEvidence* pBone = nullptr;

auto UNITY_CALLING_CONVENTION SceneManager__LoadScene(II::String* sceneName) -> void
{
	H::Fcall(SceneManager__LoadScene, sceneName);
	std::string sceneStr = std::format("{}", sceneName->ToString());
	//Log("SceneManager__LoadScene -> " + sceneStr);
}

// MainManager__OnLeftRoom
auto UNITY_CALLING_CONVENTION MainManager__OnLeftRoom(MainManager* thiz) -> void {
	H::Fcall(MainManager__OnLeftRoom, thiz);
	//Log("MainManager__OnLeftRoom");
	Game::lobbyType = MAIN;
}


/// MainManager__OnApplicationQuit
auto UNITY_CALLING_CONVENTION MainManager__OnApplicationQuit(MainManager* thiz) -> void {
	H::Fcall(MainManager__OnApplicationQuit, thiz);
	//Log("MainManager__OnApplicationQuit");
	Game::lobbyType = MAIN;
}

auto UNITY_CALLING_CONVENTION MainManager__Start(MainManager* thiz) -> void {
	//Log("MainManager__Start");

	Game::OnMissionOver();

	H::Fcall(MainManager__Start, thiz);
}

auto UNITY_CALLING_CONVENTION LevelController__HGetFavGhostRoom(LevelController* _this) -> LevelRoom*
{
	return H::Fcall(LevelController__HGetFavGhostRoom, _this);
}

auto UNITY_CALLING_CONVENTION LevelController__HAwake(LevelController* _this) -> void
{
	H::Fcall(LevelController__HAwake, _this);
	gLevelController = _this;
	//LOGD(std::format("LevelController Address: {}\n", static_cast<void*>(_this)));
}

auto UNITY_CALLING_CONVENTION RewardManager__HAwake(RewardManager* _this) -> void
{
	H::Fcall(RewardManager__HAwake, _this);
	//LOGD(std::format("RewardManager Address: {}\n", static_cast<void*>(_this)));
}

auto GetGhostFavouriteRoom() -> LevelRoom* {
	if (gLevelController && Game::isOnMission) {
		if (*(uintptr_t*)((uintptr_t)gLevelController + 0x50)) {
			// has rooms
			return *(LevelRoom**)((uintptr_t)gLevelController + 0x38);
		}
	}
	return nullptr;
}

auto UNITY_CALLING_CONVENTION PCStamina__HUpdate(void* _this) -> void
{
	if (ApplicationInfo::bCheatEnabled[CHEAT_PLAYER_ANTISTAMINA]) // good, but no footsteps sounds
		return;
	H::Fcall(PCStamina__HUpdate, _this);
}

auto UNITY_CALLING_CONVENTION EMF__Update(EMF* _this) -> void
{
	H::Fcall(EMF__Update, _this);

	if (!Ghost::gCurrentGhost || !Game::isOnMission) {
		return;
	}

	auto it = std::find_if(Ghost::emfData.begin(), Ghost::emfData.end(),
		[_this](const auto& entry) {
			return std::get<0>(entry) == _this;
		});

	if (it == Ghost::emfData.end()) {
		auto now = std::chrono::steady_clock::now();
		Ghost::emfData.push_back(std::make_tuple(_this, now));
	}
}


auto UNITY_CALLING_CONVENTION GhostModel__Show(void* _this, bool bShow) -> void
{
	if (ApplicationInfo::bCheatEnabled[CHEAT_GHOST_MODEL_SHOW] && !bShow) {
		bShow = true;
	}

	if (ApplicationInfo::bCheatEnabled[CHEAT_VISIBLE_GHOST_AT_HUNT] && !bShow && Ghost::IsHunting()) {
		bShow = true;
	}

	H::Fcall(GhostModel__Show, _this, bShow);
}


auto UNITY_CALLING_CONVENTION PlayerStamina__Update(void* _this) -> void
{
	if (_this && ApplicationInfo::bCheatEnabled[CHEAT_PLAYER_ANTISTAMINA]) {
		*(bool*)((uintptr_t)_this + 0x4A) = true;
	}

	H::Fcall(PlayerStamina__Update, _this);
}

auto UNITY_CALLING_CONVENTION DNAEvidence__Spawn(DNAEvidence* _this, int pos) -> void
{
	H::Fcall(DNAEvidence__Spawn, _this, pos);
	pBone = _this;
}

auto UNITY_CALLING_CONVENTION ConnectToServer__SingleplayerButton(void* thiz) -> bool
{
	H::Fcall(ConnectToServer__SingleplayerButton, thiz);
	//Log("Single player button clicked");
	Game::lobbyType = eLobbyType::SINGLEPLAYER;
	return true;
}

auto UNITY_CALLING_CONVENTION ConnectToServer__QuitButton(void* thiz) -> bool
{
	H::Fcall(ConnectToServer__QuitButton, thiz);
	//Log("Quit button clicked");
	Game::lobbyType = eLobbyType::MAIN;
}

// ConnectToServer__MultiplayerButton
auto UNITY_CALLING_CONVENTION ConnectToServer__MultiplayerButton(void* thiz) -> bool
{
	H::Fcall(ConnectToServer__MultiplayerButton, thiz);
	//Log("MultiPlayer button clicked");
	Game::lobbyType = eLobbyType::MULTIPLAYER;
	return true;
}

auto UNITY_CALLING_CONVENTION LobbyController__OnLeftRoom(void* thiz) -> bool
{
	H::Fcall(LobbyController__OnLeftRoom, thiz);
	//Log("LobbyController__OnLeftRoom");
	Game::lobbyType = eLobbyType::MAIN;
}



auto UNITY_CALLING_CONVENTION BrightnessChangeValue(uintptr_t _this, float val) -> void
{
	std::cout << "BrightnessChangeValue" << std::endl;
	//val *= 1000.0f;
	//_this->brightnessValue = 1000
	//*(float*)((uintptr_t)_this + 0x60) = 500.0f; // brightness value
	H::Fcall(BrightnessChangeValue, _this, val);
}


void InjectGlobal() {
	//Log("InjectGlobal");

	//setupHook("UnityEngine.CoreModule.dll", "RenderSettings", "set_ambientLight_Injected", set_ambientLight_Injected);
	//setupHook("Assembly-CSharp.dll", "GameGraphicsManager", "BrightnessChangeValue", BrightnessChangeValue);

	setupHook("Assembly-CSharp.dll", "MainManager", "Start", MainManager__Start);
	setupHook("Assembly-CSharp.dll", "MainManager", "OnLeftRoom", MainManager__OnLeftRoom);

	// GhostController__OnPlayerLeftRoom
	//setupHook("Assembly-CSharp.dll", "GhostController", "OnPlayerLeftRoom", LobbyController__OnLeftRoom);
	// LobbyController__OnLeftRoom


	//setupRVAHook((void*)0x741820, MainManager__OnApplicationQuit);
	// OnApplicationQuit
	// 
	//setupHook("Assembly-CSharp.dll", "LevelController", "get_favouriteGhostRoom", LevelController__HGetFavGhostRoom);
	setupHook("Assembly-CSharp.dll", "LevelController", "Awake", LevelController__HAwake);
	setupHook("Assembly-CSharp.dll", "EMF", "Update", EMF__Update);

	setupHook("Assembly-CSharp.dll", "DNAEvidence", "Spawn", DNAEvidence__Spawn);

	/*
void PlayerStamina_Update(PlayerStamina_o *this, const MethodInfo *method)
{
	// 1809DBEA0
	// [Address(RVA = "0x9DBEA0", Offset = "0x9DA8A0", VA = "0x1809DBEA0")]
	// got 0x9DBEA0
  PlayerStamina____________6452788896(this, 0LL);
}

*/

	setupRVAHook((void*)0x9DBEA0, PlayerStamina__Update);


	//setupRVAHook((void*)0x158D6D0, GhostModel__Show); // 0x158D6D0

	// SingleplayerProfile__Start
	// LobbyController__CheckIfSingleplayer
	setupHook("Assembly-CSharp.dll", "ConnectToServer", "SingleplayerButton", ConnectToServer__SingleplayerButton);
	//setupHook("Assembly-CSharp.dll", "ConnectToServer", "QuitButton", ConnectToServer__QuitButton);
	setupHook("Assembly-CSharp.dll", "ConnectToServer", "MultiplayerButton", ConnectToServer__MultiplayerButton);
	// MultiplayerButton


	// 
	// 
	// 0x789610
	//setupRVAHook((void*)0x789610, ServerManager__AddPlayerCard);
	
	//setupHook("Assembly-CSharp.dll", "PCStamina", "Update", PCStamina__HUpdate);
	// 0x97D470 - PlayerStamina::Update (Crypted)
	//setupRVAHook((void*)0x4187040, SceneManager__LoadScene);
	// 0x4187040
	// 
	// GhostModel$$__________6465050320   - show ghost model (GhostModel is ptr)
}

bool bOnceInjected = false;
void InjectHooks() 
{
	if (bOnceInjected) return;

	//Log("InjectHooks");

	InjectGlobal();

	//Room::Init();
	//Players::Init();
	Ghost::Init();
	bOnceInjected = true;
}