#include "../main.h"
#include "Event.h"

auto GetGhostCurrentRoom() -> LevelRoom* 
{
    if (LevelController::instance && Game::isOnMission) {
        return LevelController::instance->currentGhostRoom;
    }

    return nullptr;
}

void Events::Initialise()
{
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

    OnSetNewBansheeTarget.SubscribePost([](EventResult<void>& result, GhostAI* _this, Player* player)
    {
        if (player) {
            Ghost::SetNewBansheeTarget(player);
        }
    });

    OnLevelControllerAwake.SubscribePost([](EventResult<void>& result, LevelController* _this)
    {
        LevelController::instance = _this;
    });

    OnEMFUpdate.SubscribePost([](EventResult<void>& result, EMF* _this)
    {
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
    });

    OnCursedItemsControllerStart.SubscribePost([](EventResult<void>& result, CursedItemsController* _this)
    {
        CursedItemsController::instance = _this;
    });

    OnPlayerStaminaUpdate.SubscribePre([](EventResult<void>& result, PlayerStamina* _this)
    {
        if (_this && ApplicationInfo::bCheatEnabled[CHEAT_PLAYER_ANTISTAMINA])
        {
            if (_this->GetCurrentStamina() != 3.0f) {
                _this->SetCurrentStamina(3.0f);
            }
        }
    });

    OnMainManagerAwake.SubscribePre([](EventResult<void>& result, void* _this)
    {
        Game::OnMissionOver();
    });

    OnSpawnBoneDNAEvidence.SubscribePost([](EventResult<void>& result, EvidenceController* _this, LevelRoom* roomOpt)
    {
        
    });

    OnPhotonViewAwake.SubscribePost([](EventResult<void>& result, Photon::Pun::PhotonView* _this)
    {
        if (_this)
        {
            if (auto owner = _this->GetOwner())
            {
                Game::bPhotonAwaken = true;
            }
        }
    });

    OnPhotonViewOnDestroy.SubscribePre([](EventResult<void>& result, Photon::Pun::PhotonView* _this)
    {
        Game::bPhotonAwaken = false;
    });

    OnPlayerTeleport.SubscribePre([](EventResult<void>& result, Player* _this, II::Vector3 pos)
    {
        if (ApplicationInfo::bCheatEnabled[CHEAT_NOCLIP]) {
            result.cancel = true;
        }
    });

    OnPlayerToggleFreezePlayer.SubscribePost([](EventResult<void>& result, Player* _this, bool state)
    {
        
    });

    OnRandomRangeInt.SubscribePre([](EventResult<int32_t>& result, int32_t minInclusive, int32_t maxExclusive)
    {
        static bool bUseStaticGhost = true;
        static GhostTraits::GhostType selectedGhost = GhostTraits::GhostType::Banshee;

        if (bUseStaticGhost && minInclusive == 0 && maxExclusive == 24) {
            result.cancel = true;
            result.returnValue = static_cast<int32_t>(selectedGhost);
        }
    });

    OnMapControllerStart.SubscribePost([](EventResult<void>& result, MapController* _this)
    {
        MapController::instance = _this;
    });

    OnNetworkStart.SubscribePost([](EventResult<void*>& result, Network* _this)
    {
        Network::instance = _this;
    });

    OnGhostAIAwake.SubscribePost([](EventResult<void>& result, GhostAI* _this)
    {
        Ghost::gCurrentGhost = _this;
        Game::OnMissionStart();
    });

    OnGhostAIUpdate.SubscribePost([](EventResult<void>& result, GhostAI* _this)
    {
        
    });
}

void Events::Uninitialise()
{
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
}
