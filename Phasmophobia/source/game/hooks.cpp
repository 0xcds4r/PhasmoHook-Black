#include "../main.h"
#include "hooks.h"
#include "../utils/Event.h"

auto UNITY_CALLING_CONVENTION GhostAI__SetNewBansheeTarget(GhostAI* _this, Player* player) -> void
{
    EventResult<void> result;
    if (!Events::OnSetNewBansheeTarget.InvokePre(result, _this, player)) {
        H::Fcall(GhostAI__SetNewBansheeTarget, _this, player);
        Events::OnSetNewBansheeTarget.InvokePost(result, _this, player);
    }
}

auto UNITY_CALLING_CONVENTION LevelController__HAwake(LevelController* _this) -> void
{
    EventResult<void> result;
    if (!Events::OnLevelControllerAwake.InvokePre(result, _this)) {
        H::Fcall(LevelController__HAwake, _this);
        Events::OnLevelControllerAwake.InvokePost(result, _this);
    }
}

auto UNITY_CALLING_CONVENTION EMF__Update(EMF* _this) -> void
{
    EventResult<void> result;
    if (!Events::OnEMFUpdate.InvokePre(result, _this)) {
        H::Fcall(EMF__Update, _this);
        Events::OnEMFUpdate.InvokePost(result, _this);
    }
}

auto UNITY_CALLING_CONVENTION CursedItemsController__Start(CursedItemsController* _this) -> void
{
    EventResult<void> result;
    if (!Events::OnCursedItemsControllerStart.InvokePre(result, _this)) {
        H::Fcall(CursedItemsController__Start, _this);
        Events::OnCursedItemsControllerStart.InvokePost(result, _this);
    }
}

auto UNITY_CALLING_CONVENTION PlayerStamina__Update(PlayerStamina* _this) -> void
{
    EventResult<void> result;
    if (!Events::OnPlayerStaminaUpdate.InvokePre(result, _this)) {
        H::Fcall(PlayerStamina__Update, _this);
        Events::OnPlayerStaminaUpdate.InvokePost(result, _this);
    }
}

auto UNITY_CALLING_CONVENTION MainManager__HAwake(void* _this) -> void
{
    EventResult<void> result;
    if (!Events::OnMainManagerAwake.InvokePre(result, _this)) {
        H::Fcall(MainManager__HAwake, _this);
        Events::OnMainManagerAwake.InvokePost(result, _this);
    }
}

auto UNITY_CALLING_CONVENTION EvidenceController__SpawnBoneDNAEvidence(EvidenceController* _this, LevelRoom* roomOpt) -> void
{
    EventResult<void> result;
    if (!Events::OnSpawnBoneDNAEvidence.InvokePre(result, _this, roomOpt)) {
        H::Fcall(EvidenceController__SpawnBoneDNAEvidence, _this, roomOpt);
        Events::OnSpawnBoneDNAEvidence.InvokePost(result, _this, roomOpt);
    }
}

auto UNITY_CALLING_CONVENTION PhotonView__Awake(Photon::Pun::PhotonView* _this) -> void
{
    EventResult<void> result;
    if (!Events::OnPhotonViewAwake.InvokePre(result, _this)) {
        H::Fcall(PhotonView__Awake, _this);
        Events::OnPhotonViewAwake.InvokePost(result, _this);
    }
}

auto UNITY_CALLING_CONVENTION PhotonView__OnDestroy(Photon::Pun::PhotonView* _this) -> void
{
    EventResult<void> result;
    if (!Events::OnPhotonViewOnDestroy.InvokePre(result, _this)) {
        H::Fcall(PhotonView__OnDestroy, _this);
        Events::OnPhotonViewOnDestroy.InvokePost(result, _this);
    }
}

auto UNITY_CALLING_CONVENTION Player__Teleport(Player* _this, II::Vector3 pos) -> void
{
    EventResult<void> result;
    if (!Events::OnPlayerTeleport.InvokePre(result, _this, pos)) {
        H::Fcall(Player__Teleport, _this, pos);
        Events::OnPlayerTeleport.InvokePost(result, _this, pos);
    }
}

auto UNITY_CALLING_CONVENTION Player__ToggleFreezePlayer(Player* _this, bool state) -> void
{
    EventResult<void> result;
    if (!Events::OnPlayerToggleFreezePlayer.InvokePre(result, _this, state)) {
        H::Fcall(Player__ToggleFreezePlayer, _this, state);
        Events::OnPlayerToggleFreezePlayer.InvokePost(result, _this, state);
    }
}

inline auto UNITY_CALLING_CONVENTION RandomRangeInt(int32_t minInclusive, int32_t maxExclusive) -> int32_t
{
    EventResult<int32_t> result;
    if (!Events::OnRandomRangeInt.InvokePre(result, minInclusive, maxExclusive)) {
        int32_t value = H::Fcall(RandomRangeInt, minInclusive, maxExclusive);
        Events::OnRandomRangeInt.InvokePost(result, minInclusive, maxExclusive);
        return value;
    }
    return result.returnValue; 
}

auto UNITY_CALLING_CONVENTION MapController__Start(MapController* _this) -> void
{
    EventResult<void> result;
    if (!Events::OnMapControllerStart.InvokePre(result, _this)) {
        H::Fcall(MapController__Start, _this);
        Events::OnMapControllerStart.InvokePost(result, _this);
    }
}

auto UNITY_CALLING_CONVENTION Network__Start(Network* _this) -> void*
{
    EventResult<void*> result;
    if (!Events::OnNetworkStart.InvokePre(result, _this)) {
        void* value = H::Fcall(Network__Start, _this);
        Events::OnNetworkStart.InvokePost(result, _this);
        return value;
    }
    return result.returnValue;
}

auto UNITY_CALLING_CONVENTION GhostAI__Awake(GhostAI* _this) -> void
{
    EventResult<void> result;
    if (!Events::OnGhostAIAwake.InvokePre(result, _this)) {
        H::Fcall(GhostAI__Awake, _this);
        Events::OnGhostAIAwake.InvokePost(result, _this);
    }
}

auto UNITY_CALLING_CONVENTION GhostAI__Update(GhostAI* _this) -> void
{
    EventResult<void> result;
    if (!Events::OnGhostAIUpdate.InvokePre(result, _this)) {
        H::Fcall(GhostAI__Update, _this);
        Events::OnGhostAIUpdate.InvokePost(result, _this);
    }
}
auto UNITY_CALLING_CONVENTION Photon_Pun_PhotonView__RPC(void* _this, II::String* methodName, int32_t target, void** parameters) -> void 
{
    LOGD(std::format("Photon_Pun_PhotonView__RPC: {}", methodName->ToString()));
    H::Fcall(Photon_Pun_PhotonView__RPC, _this, methodName, target, parameters);
}

// test
auto UNITY_CALLING_CONVENTION CreateGhost(GhostController* _this) -> void {
    H::Fcall(CreateGhost, _this);
    LOGD("CreateGhost called hook");
    _this->ghostTraits.ghostType = GhostTraits::GhostType::Moroi;
    LOGD("CreateGhost called hook #1");
    if (_this->ghost) {
        if (_this->ghost->ghostInfo) {
            LOGD("CreateGhost called hook #2");
            _this->ghost->ghostInfo->ghostTraits.ghostType = GhostTraits::GhostType::Moroi;
            _this->ghost->defaultSpeed = 700.0f;
        }
    }
}

auto UNITY_CALLING_CONVENTION HuntingState__CanHunt(void* thiz) -> bool // test
{
    return false;
    //return H::Fcall(HuntingState__CheckIfWeCanKillPlayer, player, ignoreDistance);
}


#define USE_EASTER_HACKS
II::GameObject* gJackalope = nullptr;

#ifdef USE_EASTER_HACKS
struct Jackalope : public II::MonoBehaviourPun
{
    void* jackalopeDotsSignal;
    void* audioSource;
    void* eventItem;
    int32_t attackChance;
    int32_t runAwayChance;
    int32_t attackSanityChange;
    int32_t runAwaySanityChange;
    II::List<void*>* attackClip;
    II::List<void*>* runAwayClip;
    II::List<void*>* triggerHuntClip;
    II::String* attackAnimTrigger;
    II::String* runAwayAnimTrigger;
    II::String* huntAnimTrigger;
    float jackalopeAttackSpeed;
    float jackalopeRunAwaySpeed;
    float completionDelay;
    float runAwayDirectionAvoidTargetAngle;
    float runAwayGhostSpeedModifier;
    float disappearJackalopeAttackTime;
    float disappearJackalopeRunAwayTime;
    float disappearJackalopeTriggerHuntTime;
    float jackalopeDetectionTime;
    bool dropHeldItemsOnAttack;
    float forwardAxisCorrection;
    bool updateTransforms;
    bool eventComplete;
    float jackalopeDetectionTimeAccum;
    float jackalopeSpeed;
};
VALIDATE_SIZE(Jackalope, 0xA8 + STRUCT_STUCK);

auto UNITY_CALLING_CONVENTION Jackalope__Awake(Jackalope* _this) -> void // test
{
    H::Fcall(Jackalope__Awake, _this);
    gJackalope = _this->GetGameObject();
    LOGD("jackalope spawned");
}

auto UNITY_CALLING_CONVENTION Jackalope__Update(Jackalope* _this) -> void // test
{
    H::Fcall(Jackalope__Update, _this);

    if (_this->eventComplete) {
        //LOGD("easter event completed");
        gJackalope = nullptr;
        return;
    }

    gJackalope = _this->GetGameObject();
}

auto UNITY_CALLING_CONVENTION Jackalope__DisableJackalope(Jackalope* _this, float time) -> void // test
{
    H::Fcall(Jackalope__DisableJackalope, _this, time);
    gJackalope = nullptr;
    LOGD("jackalope destroyed");
}

#endif

auto UNITY_CALLING_CONVENTION ExitLevel__Exit(ExitLevel* _this, void* pData) -> void
{
    EventResult<void> result;
    if (!Events::OnExitLevel.InvokePre(result, _this, pData)) {
        H::Fcall(ExitLevel__Exit, _this, pData);
        Events::OnExitLevel.InvokePost(result, _this, pData);
    }
}

auto UNITY_CALLING_CONVENTION PauseMenuController__Leave(PauseMenuController* _this) -> void
{
	EventResult<void> result;
	if (!Events::OnPauseMenuControllerLeave.InvokePre(result, _this)) {
		H::Fcall(PauseMenuController__Leave, _this);
		Events::OnPauseMenuControllerLeave.InvokePost(result, _this);
	}
}

auto UNITY_CALLING_CONVENTION GameController__Exit(GameController* _this, void* pData) -> void
{
	EventResult<void> result;
	if (!Events::OnGameControllerExit.InvokePre(result, _this, pData)) {
		H::Fcall(GameController__Exit, _this, pData);
		Events::OnGameControllerExit.InvokePost(result, _this, pData);
	}
}

void InjectGlobal() 
{
	LOGD("InjectGlobal");

	setupHook("Assembly-CSharp.dll", "LevelController", "Awake", LevelController__HAwake);
	setupHook("Assembly-CSharp.dll", "EMF", "Update", EMF__Update);
	setupHook("Assembly-CSharp.dll", "CursedItemsController", "Start", CursedItemsController__Start);
	setupHook("Assembly-CSharp.dll", "PCStamina", "Update", PlayerStamina__Update);
	setupHook("Assembly-CSharp.dll", "MainManager", "Awake", MainManager__HAwake);
	setupHook("Assembly-CSharp.dll", "MapController", "Start", MapController__Start);
	setupHook("Assembly-CSharp.dll", "Network", "Start", Network__Start);
	setupHook("Assembly-CSharp.dll", "GhostAI", "Awake", GhostAI__Awake);
	setupHook("Assembly-CSharp.dll", "GhostAI", "Update", GhostAI__Update);
	setupHook("Assembly-CSharp.dll", "GhostAI", "SetNewBansheeTarget", GhostAI__SetNewBansheeTarget);
    setupHook("Assembly-CSharp.dll", "ExitLevel", "Exit", ExitLevel__Exit);
    setupHook("Assembly-CSharp.dll", "PauseMenuController", "Leave", PauseMenuController__Leave);
    setupHook("Assembly-CSharp.dll", "GameController", "Exit", GameController__Exit);

#ifdef USE_EASTER_HACKS
    //setupHook("Assembly-CSharp.dll", "Jackalope", "Awake", Jackalope__Awake);
    setupHook("Assembly-CSharp.dll", "Jackalope", "Update", Jackalope__Update);
    //setupHook("Assembly-CSharp.dll", "Jackalope", "DisableJackalope", Jackalope__DisableJackalope);
    // 

#endif

    //setupHook("Assembly-CSharp.dll", "HuntingState", "CanHunt", HuntingState__CanHunt); // test
    //setupRVAHook((void*)0x762280, CreateGhost); // test

    //setupHook("Assembly-CSharp.dll", "ExtensionMethods", "GetRandom", GetRandom);
        
    // outcoming rpcs
    //setupHook("PhotonUnityNetworking.dll", "PhotonView", "RPC", Photon_Pun_PhotonView__RPC);

    //setupHook("UnityEngine.CoreModule.dll", "Random", "RandomRangeInt", RandomRangeInt); // TODO: static ghost type for single

	//setupHook("Assembly-CSharp.dll", "EvidenceController", "SpawnBoneDNAEvidence", EvidenceController__SpawnBoneDNAEvidence);
//setupHook("Assembly-CSharp.dll", "Player", "Teleport", Player__Teleport);
//setupHook("Assembly-CSharp.dll", "Player", "ToggleFreezePlayer", Player__ToggleFreezePlayer);
// Photon_Pun_PhotonView__Awake
//setupHook("PhotonUnityNetworking.dll", "PhotonView", "Awake", PhotonView__Awake);
//setupHook("PhotonUnityNetworking.dll", "PhotonView", "OnDestroy", PhotonView__OnDestroy);
//setupHook("Assembly-CSharp.dll", "EvidenceController", "SpawnBoneDNAEvidence", EvidenceController__SpawnBoneDNAEvidence);
//setupHook("Assembly-CSharp.dll", "EvidenceController", "SpawnBoneDNAEvidence", EvidenceController__SpawnBoneDNAEvidence);
    //setupHook("UnityEngine.CoreModule.dll", "Random", "RandomRangeInt", RandomRangeInt);
    // 
}

bool bOnceInjected = false;
void InjectHooks() 
{
	if (bOnceInjected) return;
		
	LOGD("Injecting hooks");
	InjectGlobal();

	//Room::Init();
	//Players::Init();
	//Ghost::Init();
	bOnceInjected = true;
}

//setupHook("Assembly-CSharp.dll", "MainManager", "Start", MainManager__Start);
// 
//auto UNITY_CALLING_CONVENTION MainManager__Start(MainManager* thiz) -> void {
//	Game::OnMissionOver();
//	Log("MainManager__Start #1");
//	H::Fcall(MainManager__Start, thiz);
//	Log("MainManager__Start #2");
//}

//setupHook("Assembly-CSharp.dll", "EvidenceController", "SpawnBoneDNAEvidence", EvidenceController__SpawnBoneDNAEvidence);
	//setupHook("Assembly-CSharp.dll", "LobbyController", "SetUpRoom", LobbyController__SetUpRoom);
	// LobbyController Start


	//setupHook("Assembly-CSharp.dll", "Network", "AuthoriseMultiplayerFeatures", Network__AuthoriseMultiplayerFeatures);

	//setupHook("Assembly-CSharp.dll", "MainManager", "OnLeftRoom", MainManager__OnLeftRoom);
	//setupHook("Assembly-CSharp.dll", "SingleplayerProfile", "Start", SingleplayerProfile__Start);
	//setupHook("Assembly-CSharp.dll", "MultiplayerProfile", "Start", MultiplayerProfile__Start);
	//setupHook("Assembly-CSharp.dll", "Network", "AuthoriseMultiplayerFeatures", AuthoriseMultiplayerFeatures);
	// AuthoriseMultiplayerFeatures Network
	// 
	// 
	// 
	// todo
	//setupHook("Assembly-CSharp.dll", "GhostAI", "SetNewBansheeTarget", GhostAI__SetNewBansheeTarget);
		// GhostAI SetNewBansheeTarget
	// PCStamina -- > PlayerStaina eq
	//setupRVAHook((void*)0x9DBEA0, PlayerStamina__Update);
	/*
void PlayerStamina_Update(PlayerStamina_o *this, const MethodInfo *method)
{
	// 1809DBEA0
	// [Address(RVA = "0x9DBEA0", Offset = "0x9DA8A0", VA = "0x1809DBEA0")]
	// got 0x9DBEA0
  PlayerStamina____________6452788896(this, 0LL);
}

*/
//setupRVAHook((void*)0x158D6D0, GhostModel__Show); // 0x158D6D0

// SingleplayerProfile__Start
// LobbyController__CheckIfSingleplayer
	// MultiplayerButton
	// //setupHook("Assembly-CSharp.dll", "ConnectToServer", "QuitButton", ConnectToServer__QuitButton);
	//setupHook("Assembly-CSharp.dll", "DNAEvidence", "Spawn", DNAEvidence__Spawn);
	//setupHook("Assembly-CSharp.dll", "DNAEvidence", "Start", DNAEvidence__Start);
	// EvidenceController SpawnBoneDNAEvidence
	//Log("InjectGlobal");

	//setupHook("UnityEngine.CoreModule.dll", "RenderSettings", "set_ambientLight_Injected", set_ambientLight_Injected);
	//setupHook("Assembly-CSharp.dll", "GameGraphicsManager", "BrightnessChangeValue", BrightnessChangeValue);
		// GhostController__OnPlayerLeftRoom
	//setupHook("Assembly-CSharp.dll", "GhostController", "OnPlayerLeftRoom", LobbyController__OnLeftRoom);
	// LobbyController__OnLeftRoom


	//setupRVAHook((void*)0x741820, MainManager__OnApplicationQuit);
	// OnApplicationQuit
	// 
	//setupHook("Assembly-CSharp.dll", "LevelController", "get_favouriteGhostRoom", LevelController__HGetFavGhostRoom);
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

//auto UNITY_CALLING_CONVENTION SceneManager__LoadScene(II::String* sceneName) -> void
//{
//	H::Fcall(SceneManager__LoadScene, sceneName);
//	//std::string sceneStr = std::format("{}", sceneName->ToString());
//	//std::cout << "[SceneManager__LoadScene] -> " << sceneStr << std::endl;
//}
//
//auto UNITY_CALLING_CONVENTION MainManager__OnLeftRoom(MainManager* thiz) -> void {
//	H::Fcall(MainManager__OnLeftRoom, thiz);
//	Game::lobbyType = MAIN;
//}
//
//auto UNITY_CALLING_CONVENTION MainManager__OnApplicationQuit(MainManager* thiz) -> void {
//	H::Fcall(MainManager__OnApplicationQuit, thiz);
//	Game::lobbyType = MAIN;
//}
//
//
//
//auto UNITY_CALLING_CONVENTION LobbyController__SetUpRoom(void* thiz) -> void {
//	H::Fcall(LobbyController__SetUpRoom, thiz);
//	std::cout << "LobbyController__SetUpRoom" << std::endl;
//}
//
//
//
//auto UNITY_CALLING_CONVENTION LevelController__HGetFavGhostRoom(LevelController* _this) -> LevelRoom*
//{
//	return H::Fcall(LevelController__HGetFavGhostRoom, _this);
//}
//
//
//
//auto UNITY_CALLING_CONVENTION RewardManager__HAwake(RewardManager* _this) -> void
//{
//	H::Fcall(RewardManager__HAwake, _this);
//}
//
//auto GetGhostFavouriteRoom() -> LevelRoom* {
//	if (gLevelController && Game::isOnMission) {
//		if (*(uintptr_t*)((uintptr_t)gLevelController + 0x50)) {
//			// has rooms
//			return *(LevelRoom**)((uintptr_t)gLevelController + 0x38);
//		}
//	}
//	return nullptr;
//}
//
//auto UNITY_CALLING_CONVENTION PCStamina__HUpdate(void* _this) -> void
//{
//	if (ApplicationInfo::bCheatEnabled[CHEAT_PLAYER_ANTISTAMINA]) // good, but no footsteps sounds
//		return;
//	H::Fcall(PCStamina__HUpdate, _this);
//}
//
//
//
//
//auto UNITY_CALLING_CONVENTION GhostModel__Show(void* _this, bool bShow) -> void
//{
//	if (ApplicationInfo::bCheatEnabled[CHEAT_GHOST_MODEL_SHOW] && !bShow) {
//		bShow = true;
//	}
//
//	if (ApplicationInfo::bCheatEnabled[CHEAT_VISIBLE_GHOST_AT_HUNT] && !bShow && Ghost::IsHunting()) {
//		bShow = true;
//	}
//
//	H::Fcall(GhostModel__Show, _this, bShow);
//}
//
//
//
//
//auto UNITY_CALLING_CONVENTION DNAEvidence__Spawn(DNAEvidence* _this, int pos) -> void
//{
//	H::Fcall(DNAEvidence__Spawn, _this, pos);
//	pBone = _this;
//}
//
//auto UNITY_CALLING_CONVENTION DNAEvidence__Start(DNAEvidence* _this) -> void
//{
//	H::Fcall(DNAEvidence__Start, _this);
//	pBone = _this;
//}
//
//auto UNITY_CALLING_CONVENTION SingleplayerProfile__Start(void* thiz) -> void
//{
//	H::Fcall(SingleplayerProfile__Start, thiz);
//	Game::lobbyType = eLobbyType::SINGLEPLAYER;
//	std::cout << "Single player profile start" << std::endl;
//}
//
//auto UNITY_CALLING_CONVENTION ConnectToServer__QuitButton(void* thiz) -> bool
//{
//	H::Fcall(ConnectToServer__QuitButton, thiz);
//	Game::lobbyType = eLobbyType::MAIN;
//}
//
//auto UNITY_CALLING_CONVENTION AuthoriseMultiplayerFeatures(void* thiz, int action) -> void
//{
//	H::Fcall(AuthoriseMultiplayerFeatures, thiz, action);
//	Game::lobbyType = eLobbyType::MULTIPLAYER;
//	std::cout << "MultiPlayer Network profile start" << std::endl;
//}
//
//auto UNITY_CALLING_CONVENTION LobbyController__OnLeftRoom(void* thiz) -> bool
//{
//	H::Fcall(LobbyController__OnLeftRoom, thiz);
//	Game::lobbyType = eLobbyType::MAIN;
//}
//
//auto UNITY_CALLING_CONVENTION BrightnessChangeValue(uintptr_t _this, float val) -> void
//{
//	H::Fcall(BrightnessChangeValue, _this, val);
//}
//
//auto UNITY_CALLING_CONVENTION EvidenceController__SpawnBoneDNAEvidence(EvidenceController* _this, LevelRoom* roomOpt) -> void
//{
//	H::Fcall(EvidenceController__SpawnBoneDNAEvidence, _this, roomOpt);
//	pBone = _this->pDNAEvidence;
//}
//
//auto UNITY_CALLING_CONVENTION GhostAI__SetNewBansheeTarget(GhostAI* _this, Player* playerOpt) -> void
//{
//	H::Fcall(GhostAI__SetNewBansheeTarget, _this, playerOpt);
//}
//
//auto UNITY_CALLING_CONVENTION Network__AuthoriseMultiplayerFeatures(void* _this, bool t) -> void
//{
//	std::cout << "Network__AuthoriseMultiplayerFeatures" << std::endl;
//	H::Fcall(Network__AuthoriseMultiplayerFeatures, _this, t);
//}