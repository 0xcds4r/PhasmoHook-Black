#pragma once

// 0.12.1.0 [+]

class LevelController;
class GameController : public II::MonoBehaviourPun {
public:
    void* OnAllPlayersConnected;
    void* OnPlayerSpawned;
    void* OnGhostSpawned;
    void* OnHuntStarted;
    void* OnHuntEnded;
    void* OnPlayerChangedRoom;
    void* OnGhostChangedRoom;
    void* OnLocalPlayerExitTruck;
    void* OnLocalPlayerEnterTruck;
    void* OnPlayerDied;
    void* OnLocalPlayerDied;
    void* OnFuseBoxUse;
    void* OnPlayerEnteredOrExitTruck;
    void* OnPhotoTaken;
    void* OnPhotoRemoved;
    void* OnPhotoVote;
    float currentAverageSanity;
    void* speechRecognitionController;
    LevelController* levelController;
    MultiplayerController* multiplayerController; 
    bool allPlayersAreConnected;
    int32_t playerReadyCount;
    II::Material* handMaterial;
    bool hasDrawnHighPriestess;
    void* speechRecognizer;
};
VALIDATE_SIZE(GameController, 0xD8 + STRUCT_STUCK);