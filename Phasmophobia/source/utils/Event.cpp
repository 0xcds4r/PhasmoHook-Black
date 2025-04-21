#include "../main.h"
#include "Event.h"
#include <format>
auto GetGhostCurrentRoom() -> LevelRoom* 
{
    if (LevelController::instance && Game::isOnMission) {
        return LevelController::instance->currentGhostRoom;
    }

    return nullptr;
}

void DoCollectEMFData(EMF* _this)
{
    if (!_this || !GhostAI::instance || !Game::isOnMission) {
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

void Events::SetupEventNames()
{
    LOGD("Events::SetupEventNames");
    OnSetNewBansheeTarget.SetName("OnSetNewBansheeTarget");
    OnLevelControllerAwake.SetName("OnLevelControllerAwake");
    OnEMFUpdate.SetName("OnEMFUpdate");
    OnCursedItemsControllerStart.SetName("OnCursedItemsControllerStart");
    OnPlayerStaminaUpdate.SetName("OnPlayerStaminaUpdate");
    OnMainManagerAwake.SetName("OnMainManagerAwake");
    OnSpawnBoneDNAEvidence.SetName("OnSpawnBoneDNAEvidence");
    OnPhotonViewAwake.SetName("OnPhotonViewAwake");
    OnPhotonViewOnDestroy.SetName("OnPhotonViewOnDestroy");
    OnPlayerTeleport.SetName("OnPlayerTeleport");
    OnPlayerToggleFreezePlayer.SetName("OnPlayerToggleFreezePlayer");
    OnRandomRangeInt.SetName("OnRandomRangeInt");
    OnMapControllerStart.SetName("OnMapControllerStart");
    OnNetworkStart.SetName("OnNetworkStart");
    OnGhostAIAwake.SetName("OnGhostAIAwake");
    OnGhostAIUpdate.SetName("OnGhostAIUpdate");
    OnExitLevel.SetName("OnExitLevel");
    OnGameControllerExit.SetName("OnGameControllerExit");
    OnPauseMenuControllerLeave.SetName("OnPauseMenuControllerLeave");
	OnObjectiveManagerStart.SetName("OnObjectiveManagerStart");
	OnAddSideObjective.SetName("OnAddSideObjective");
}

void Events::SetupInstances() 
{
    LOGD("Events::SetupInstances");
    OnLevelControllerAwake.SubscribePost([](EventResult<void>& result, LevelController* _this)
    {
        LevelController::instance = _this;
    });

    OnCursedItemsControllerStart.SubscribePost([](EventResult<void>& result, CursedItemsController* _this)
    {
        CursedItemsController::instance = _this;
    });

    OnMapControllerStart.SubscribePost([](EventResult<void>& result, MapController* _this)
    {
        MapController::instance = _this;
    });

    OnNetworkStart.SubscribePost([](EventResult<void*>& result, Network* _this)
    {
        Network::instance = _this;
    });

	OnObjectiveManagerStart.SubscribePost([](EventResult<void>& result, ObjectiveManager* _this)
	{
		ObjectiveManager::instance = _this;

        if (ApplicationInfo::bCheatEnabled[CHEAT_MAXREWARD]) {
            if(_this) 
                _this->CompleteAllObjectives();
        }
	});

	OnAddSideObjective.SubscribePost([](EventResult<void>& result, ObjectiveManager* _this, Objective* objective, int32_t id)
	{
            LOGD("OnAddSideObjective");
		//if (ApplicationInfo::bCheatEnabled[CHEAT_MAXREWARD]) {
			//LOGD(std::format("Events::OnAddSideObjective -> {} (Completed success!)", objective->name->ToString()));
			//objective->completed = true;
		//}
	});
}

void Events::ResetInstances() {
    LOGD("Events::ResetInstances");
	LevelController::instance = nullptr;
	CursedItemsController::instance = nullptr;
	MapController::instance = nullptr;
	//Network::instance = nullptr;
    GhostAI::instance = nullptr;
	ObjectiveManager::instance = nullptr;
}

void Events::SetupMissionOverEvents()
{
    LOGD("Events::SetupMissionOverEvents");

    OnMainManagerAwake.SubscribePre([](EventResult<void>& result, void* _this)
    {
        Game::OnMissionOver();
    });

    OnExitLevel.SubscribePre([](EventResult<void>& result, ExitLevel* _this, void* pData)
    {
        //LOGD("OnExitLevel PRE");
    });

    OnExitLevel.SubscribePost([](EventResult<void>& result, ExitLevel* _this, void* pData)
    {
        //LOGD("OnExitLevel POST");
        Game::OnMissionOver();
    });

    OnGameControllerExit.SubscribePre([](EventResult<void>& result, GameController* _this, void* pData)
    {
        //LOGD("OnGameControllerExit PRE");
    });

    OnGameControllerExit.SubscribePost([](EventResult<void>& result, GameController* _this, void* pData)
    {
        //LOGD("OnGameControllerExit Post");
        Game::OnMissionOver();
    });

    OnPauseMenuControllerLeave.SubscribePre([](EventResult<void>& result, PauseMenuController* _this)
    {
        //LOGD("OnPauseMenuControllerLeave PRE");
    });

    OnPauseMenuControllerLeave.SubscribePost([](EventResult<void>& result, PauseMenuController* _this)
    {
        //LOGD("OnPauseControllerLeave POST");
        Game::OnMissionOver();
    });
}

void Events::SetupPlayerEvents() 
{
    LOGD("Events::SetupPlayerEvents");

    OnPlayerStaminaUpdate.SubscribePre([](EventResult<void>& result, PlayerStamina* _this)
    {
        if (_this && ApplicationInfo::bCheatEnabled[CHEAT_PLAYER_ANTISTAMINA])
        {
            if (_this->GetCurrentStamina() != 3.0f) {
                _this->SetCurrentStamina(3.0f);
            }
        }
    });

    OnPlayerTeleport.SubscribePre([](EventResult<void>& result, Player* _this, II::Vector3 pos)
    {
        if (ApplicationInfo::bCheatEnabled[CHEAT_NOCLIP]) {
            result.cancel = true;
        }
    });
}

void Events::SetupGhostEvents() {
    LOGD("Events::SetupGhostEvents");

    OnGhostAIAwake.SubscribePost([](EventResult<void>& result, GhostAI* _this)
    {
        GhostAI::instance = _this;
        Game::OnMissionStart();
    });

    OnGhostAIUpdate.SubscribePost([](EventResult<void>& result, GhostAI* _this)
    {
        GhostAI::instance = _this;
    });

    OnSetNewBansheeTarget.SubscribePost([](EventResult<void>& result, GhostAI* _this, Player* player)
    {
        if (player) {
            Ghost::SetNewBansheeTarget(player);
        }
    });

    OnEMFUpdate.SubscribePost([](EventResult<void>& result, EMF* _this)
    {
        DoCollectEMFData(_this);
    });
}

void Events::Initialise()
{
	LOGD("Initialise Events..");
    SetupEventNames();
    SetupInstances();
    SetupMissionOverEvents();
    SetupPlayerEvents();
    SetupGhostEvents();
}

void Events::Uninitialise()
{
	LOGD("Uninitialise Events..");

    OnSetNewBansheeTarget.Clear();
    OnLevelControllerAwake.Clear();
    OnEMFUpdate.Clear();
    OnCursedItemsControllerStart.Clear();
    OnPlayerStaminaUpdate.Clear();
    OnMainManagerAwake.Clear();
    OnSpawnBoneDNAEvidence.Clear();
    OnPhotonViewAwake.Clear();
    OnPhotonViewOnDestroy.Clear();
    OnPlayerTeleport.Clear();
    OnPlayerToggleFreezePlayer.Clear();
    OnRandomRangeInt.Clear();
    OnMapControllerStart.Clear();
    OnNetworkStart.Clear();
    OnGhostAIAwake.Clear();
    OnGhostAIUpdate.Clear();
    OnExitLevel.Clear();
	OnGameControllerExit.Clear();
	OnPauseMenuControllerLeave.Clear();
	OnObjectiveManagerStart.Clear();
    OnAddSideObjective.Clear();
}
