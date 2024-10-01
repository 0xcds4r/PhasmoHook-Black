#include "../main.h"

LevelController* gLevelController = nullptr;

auto UNITY_CALLING_CONVENTION SceneManager__LoadScene(II::String* sceneName) -> void
{
	H::Fcall(SceneManager__LoadScene, sceneName);
	std::string sceneStr = std::format("{}", sceneName->ToString());
	Log("SceneManager__LoadScene -> " + sceneStr);
}

auto UNITY_CALLING_CONVENTION MainManager__Start(MainManager* thiz) -> void {
	//LOGD("MainManager__Start");
	
	// reset all data here
	Ghost::Reset();
	Room::Reset();
	Players::Reset();

	gLevelController = nullptr;
	Gui::pBone = nullptr;

	ApplicationInfo::bIsInLobby = true;

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
	if (gLevelController) {
		return LevelController__HGetFavGhostRoom(gLevelController);
	}
	return nullptr;
}
auto UNITY_CALLING_CONVENTION CharacterController__set_radius(void* _this, float radius) -> void
{
	H::Fcall(CharacterController__set_radius, _this, INFINITY);
}

auto UNITY_CALLING_CONVENTION SimpleMove_Injected(void* _this, II::Vector3* vec) -> void
{
	H::Fcall(SimpleMove_Injected, _this, vec);
	//LOGD("SimpleMove_Injected calling");
	CharacterController__set_radius(_this, INFINITY);
}

auto UNITY_CALLING_CONVENTION Move_Injected(void* _this, II::Vector3* vec) -> void
{
	H::Fcall(Move_Injected, _this, vec);
	//LOGD("Move_Injected calling");
	CharacterController__set_radius(_this, INFINITY);
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

	if (!Ghost::gCurrentGhost) {
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
	if (ApplicationInfo::bCheatEnabled[CHEAT_PLAYER_ANTISTAMINA]) {
		*(bool*)((uintptr_t)_this + 0x4A) = true;
	}

	H::Fcall(PlayerStamina__Update, _this);
}

auto UNITY_CALLING_CONVENTION DNAEvidence__Spawn(DNAEvidence* _this, int pos) -> void
{
	H::Fcall(DNAEvidence__Spawn, _this, pos);
	Gui::pBone = _this;
}

void InjectGlobal() {
	Log("InjectGlobal");

	setupHook("Assembly-CSharp.dll", "MainManager", "Start", MainManager__Start);
	setupHook("Assembly-CSharp.dll", "LevelController", "get_favouriteGhostRoom", LevelController__HGetFavGhostRoom);
	setupHook("Assembly-CSharp.dll", "LevelController", "Awake", LevelController__HAwake);
	setupHook("Assembly-CSharp.dll", "EMF", "Update", EMF__Update);

	setupHook("Assembly-CSharp.dll", "DNAEvidence", "Spawn", DNAEvidence__Spawn);

	setupRVAHook((void*)0x97D470, PlayerStamina__Update);
	setupRVAHook((void*)0x158D6D0, GhostModel__Show); // 0x158D6D0
	

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

	Log("InjectHooks");

	InjectGlobal();

	Room::Init();
	Players::Init();
	Ghost::Init();
	bOnceInjected = true;
}