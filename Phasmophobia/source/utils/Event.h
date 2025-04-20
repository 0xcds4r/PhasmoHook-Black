#pragma once

#include <functional>
#include <vector>
#include "../../library/PhotonHelper.h"

template<typename ReturnType>
struct EventResult {
    bool cancel = false;
    ReturnType returnValue{}; 
};

template<>
struct EventResult<void> {
    bool cancel = false;
};

template<typename ReturnType, typename... Args>
class Event {
public:
    using Callback = std::function<void(EventResult<ReturnType>&, Args...)>;

    void SetName(const std::string& eventName) {
        name = eventName;
    }

    void SubscribePre(Callback callback) {
        preCallbacks.push_back(std::move(callback));
    }

    void SubscribePost(Callback callback) {
        postCallbacks.push_back(std::move(callback));
    }

    bool InvokePre(EventResult<ReturnType>& result, Args... args) 
    {
        result.cancel = false;
        for (size_t i = 0; i < preCallbacks.size(); ++i) {
            preCallbacks[i](result, args...);
            if (result.cancel) {
                return true;
            }
        }
        return false;
    }

    void InvokePost(EventResult<ReturnType>& result, Args... args) {
        for (size_t i = 0; i < postCallbacks.size(); ++i) {
            postCallbacks[i](result, args...);
        }
    }

    void Clear() {
        preCallbacks.clear();
        postCallbacks.clear();
    }

private:
    std::vector<Callback> preCallbacks;
    std::vector<Callback> postCallbacks;
    std::string name = "Undefined";
};

class Events {
public:
    inline static Event<void, GhostAI*, Player*> OnSetNewBansheeTarget{};
    inline static Event<void, LevelController*> OnLevelControllerAwake{};
    inline static Event<void, EMF*> OnEMFUpdate{};
    inline static Event<void, CursedItemsController*> OnCursedItemsControllerStart{};
    inline static Event<void, PlayerStamina*> OnPlayerStaminaUpdate{};
    inline static Event<void, void*> OnMainManagerAwake{};
    inline static Event<void, EvidenceController*, LevelRoom*> OnSpawnBoneDNAEvidence{};
    inline static Event<void, Photon::Pun::PhotonView*> OnPhotonViewAwake{};
    inline static Event<void, Photon::Pun::PhotonView*> OnPhotonViewOnDestroy{};
    inline static Event<void, Player*, II::Vector3> OnPlayerTeleport{};
    inline static Event<void, Player*, bool> OnPlayerToggleFreezePlayer{};
    inline static Event<int32_t, int32_t, int32_t> OnRandomRangeInt{};
    inline static Event<void, MapController*> OnMapControllerStart{};
    inline static Event<void*, Network*> OnNetworkStart{};
    inline static Event<void, GhostAI*> OnGhostAIAwake{};
    inline static Event<void, GhostAI*> OnGhostAIUpdate{};
    inline static Event<void, ExitLevel*, void*> OnExitLevel{};
	inline static Event<void, PauseMenuController*> OnPauseMenuControllerLeave{};
	inline static Event<void, GameController*, void*> OnGameControllerExit{};

    static void Initialise();
    static void Uninitialise();
    static void SetupInstances();
    static void SetupEventNames();
    static void SetupMissionOverEvents();
    static void SetupPlayerEvents();
    static void SetupGhostEvents();
    static void ResetInstances();
};


