#pragma once

// 0.12.1.0 [+]

class MainManager : public II::MonoBehaviourPun {
public:
    void* sceneCamera;
    void* mainUICanvasGroup;
    II::List<II::Transform*>* spawns;
    II::Transform* psvr2EulaSpawn;
    bool ranOnce;
    void* serverManager;
    void* levelSelection;
    void* ghostOS;
    void* errorManager;
    void* serverVersionText;
    void* unityGamingServicesIDText;
    void* eulaController;
    II::Material* handMaterial;
    II::GameObject* quitButton;
    void* eulaSignal;
    void* firstLoadSignal;
    void* serverSignal;
    void* lobbySignal;
    void* finishGameSignal;
    void* finishTrainingSignal;
    void* loadMapSignal;
    void* legacyBadgeSignal;
    void* psvr2ControlUISignal;
    void* OnDifficultyChanged;
};
VALIDATE_SIZE(MainManager, 0xD8 + STRUCT_STUCK);