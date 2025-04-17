#pragma once

class GhostTraits
{
public:
    enum class GhostType : std::int32_t {
        Spirit,
        Wraith,
        Phantom,
        Poltergeist,
        Banshee,
        Jinn,
        Mare,
        Revenant,
        Shade,
        Demon,
        Yurei,
        Oni,
        Yokai,
        Hantu,
        Goryo,
        Myling,
        Onryo,
        TheTwins,
        Raiju,
        Obake,
        Mimic,
        Moroi,
        Deogen,
        Thaye
    };

    GhostType ghostType;            // Offset: 0x0 
    GhostType mimicType;   // Offset: 0x4 
    II::List<Proof>* evidenceList; // Offset: 0x8 
    II::List<Proof>* optionalEvidenceList; // Offset: 0x10 
    int age;         // Offset: 0x18 
    bool bIsMale;                    // Offset: 0x1C
    II::String* ghostName;           // Offset: 0x20 
    int ghostFirstNameID;             // Offset: 0x28 
    int ghostLastNameID;             // Offset: 0x2C 
    bool bIsShy;               // Offset: 0x30
    int deathLength;                    // Offset: 0x34
    int favouriteRoomID;                    // Offset: 0x38
    bool hasSpecialAbility;          // Offset: 0x3C 
};
VALIDATE_SIZE(GhostTraits, 0x40);

class GhostAI;
class GhostInfo : public II::MonoBehaviourPun {
public:
    GhostTraits ghostTraits;    // Offset: 0x28 [HideInInspector] 
    GhostAI* ghost;              // Offset: 0x68 [SerializeField]
    LevelRoom* favouriteRoom;    // Offset: 0x70 [HideInInspector]
    float activityMultiplier;            // Offset: 0x78 [HideInInspector] 
    bool isInitialized;          // Offset: 0x7C
};
VALIDATE_SIZE(GhostInfo, 0x70 + STRUCT_STUCK);

class GhostAI : public II::MonoBehaviourPun {
public:
    enum class State : std::int64_t {
        idle,
        wander,
        hunting,
        favouriteRoom,
        light,
        door,
        throwing,
        fusebox,
        appear,
        doorKnock,
        windowKnock,
        carAlarm,
        flicker,
        cctv,
        randomEvent,
        GhostAbility,
        mannequin,
        teleportObject,
        interact,
        summoningCircle,
        musicBox,
        dots,
        salt,
        ignite
    };

    void* levelController;  // 0x28 - internal readonly
    GhostAI::State state; // 0x30
    GhostInfo* ghostInfo; // 0x38
    void* navMeshAgent; // 0x40
    void* ghostAudio;
    void* ghostInteraction;
    void* ghostActivity;
    void* currentGhostModel;
    void* halloweenModel;                  // Offset: 0x68 [SerializeField]
    void* holidayModel;                    // Offset: 0x70 [SerializeField]
    void* easterModel;                     // Offset: 0x78 [SerializeField]
    void** normalModels;                   // Offset: 0x80 (array)
    void** specialModels;                  // Offset: 0x88 (array)
    bool isActive;                               // Offset: 0x90
    II::Rendering::ShadowCastingMode shadowMode;
    II::List<II::Vector3>* waypoints; // *
    float wanderTimer;
    void* losSensor;
    bool bAppear; // 0xB0 ??
    II::Transform* headTransform;       // Offset: 0xB8
    II::Transform* bodyTransform;       // Offset: 0xC0
    II::Transform* feetTransform;       // Offset: 0xC8
    float speed;                             // Offset: 0xD0 [HideInInspector]
    float field_1;                   // Offset: 0xD4 [HideInInspector]
    float field_2;                       // Offset: 0xD8 [HideInInspector]
    bool field_3;                            // Offset: 0xDC [HideInInspector]
    bool field_4;                               // Offset: 0xDD [HideInInspector]
    II::Vector3 lastKnownPosition;      // Offset: 0xE0 [HideInInspector]
    II::GameObject* ghostObject;        // Offset: 0xF0
    bool canEnterHuntingMode;                              // Offset: 0xF8 [HideInInspector]
    bool isHunting;                              // Offset: 0xF9 [HideInInspector]
    bool bCanAttack;                          // Offset: 0xFA [HideInInspector] ??
    bool bIncensed;                              // Offset: 0xFB [HideInInspector]
    void* whiteSage;                        // Offset: 0x100 [HideInInspector]
    float chaseDuration;                         // Offset: 0x108
    bool field_8;                               // Offset: 0x10C [HideInInspector]
    bool bCanFlash;                              // Offset: 0x10D [HideInInspector]
    bool field_9;                           // Offset: 0x10E [HideInInspector]
    Player* bansheeTarget; // Offset: 0x110 [HideInInspector]
    int huntCount;                               // Offset: 0x118 [HideInInspector]
    II::Vector3 targetPosition;         // Offset: 0x11C [HideInInspector]
    float* wanderSpeeds;                         // Offset: 0x128 (readonly массив)
    float* chaseSpeeds;                          // Offset: 0x130 (readonly массив)
    float* patrolTimes;                          // Offset: 0x138 (readonly массив)
    int patrolIndex;                             // Offset: 0x140
    int waypointIndex;                           // Offset: 0x144
    int eventCounter;                            // Offset: 0x148
    int flashCounter;                            // Offset: 0x14C
    int chaseTargetIndex;                        // Offset: 0x150
    float* eventDelays;                          // Offset: 0x158 (readonly массив)
    float* actionTimes;                          // Offset: 0x160 (readonly массив)
    float* huntDurations;                        // Offset: 0x168 (readonly массив)
    float stunTimer;                             // Offset: 0x170
    int* normalModelIndices;                     // Offset: 0x178 (readonly массив)
    int* specialModelIndices;                    // Offset: 0x180 (readonly массив)
    int* huntIndices;                            // Offset: 0x188 (readonly массив)
    int* eventIndices;                           // Offset: 0x190 (readonly массив)
};
VALIDATE_SIZE(GhostAI, 0x188 + STRUCT_STUCK);

const std::map<GhostTraits::GhostType, const char*> GhostTypeNames = {
    {GhostTraits::GhostType::Spirit, "Spirit"},
    {GhostTraits::GhostType::Wraith, "Wraith"},
    {GhostTraits::GhostType::Phantom, "Phantom"},
    {GhostTraits::GhostType::Poltergeist, "Poltergeist"},
    {GhostTraits::GhostType::Banshee, "Banshee"},
    {GhostTraits::GhostType::Jinn, "Jinn"},
    {GhostTraits::GhostType::Mare, "Mare"},
    {GhostTraits::GhostType::Revenant, "Revenant"},
    {GhostTraits::GhostType::Shade, "Shade"},
    {GhostTraits::GhostType::Demon, "Demon"},
    {GhostTraits::GhostType::Yurei, "Yurei"},
    {GhostTraits::GhostType::Oni, "Oni"},
    {GhostTraits::GhostType::Yokai, "Yokai"},
    {GhostTraits::GhostType::Hantu, "Hantu"},
    {GhostTraits::GhostType::Goryo, "Goryo"},
    {GhostTraits::GhostType::Myling, "Myling"},
    {GhostTraits::GhostType::Onryo, "Onryo"},
    {GhostTraits::GhostType::TheTwins, "TheTwins"},
    {GhostTraits::GhostType::Raiju, "Raiju"},
    {GhostTraits::GhostType::Obake, "Obake"},
    {GhostTraits::GhostType::Mimic, "Mimic"},
    {GhostTraits::GhostType::Moroi, "Moroi"},
    {GhostTraits::GhostType::Deogen, "Deogen"},
    {GhostTraits::GhostType::Thaye, "Thaye"}
};

const std::map<GhostTraits::GhostType, const char*> GhostTypeNamesRus = {
    {GhostTraits::GhostType::Spirit, "Дух"},
    {GhostTraits::GhostType::Wraith, "Мираж"},
    {GhostTraits::GhostType::Phantom, "Фантом"},
    {GhostTraits::GhostType::Poltergeist, "Полтергейст"},
    {GhostTraits::GhostType::Banshee, "Банши"},
    {GhostTraits::GhostType::Jinn, "Джинн"},
    {GhostTraits::GhostType::Mare, "Мара"},
    {GhostTraits::GhostType::Revenant, "Ревенант"},
    {GhostTraits::GhostType::Shade, "Тень"},
    {GhostTraits::GhostType::Demon, "Демон"},
    {GhostTraits::GhostType::Yurei, "Юрэй"},
    {GhostTraits::GhostType::Oni, "Они"},
    {GhostTraits::GhostType::Yokai, "Ёкай"},
    {GhostTraits::GhostType::Hantu, "Ханту"},
    {GhostTraits::GhostType::Goryo, "Горё"},
    {GhostTraits::GhostType::Myling, "Мюлинг"},
    {GhostTraits::GhostType::Onryo, "Онрё"},
    {GhostTraits::GhostType::TheTwins, "Близнецы"},
    {GhostTraits::GhostType::Raiju, "Райдзю"},
    {GhostTraits::GhostType::Obake, "Обаке"},
    {GhostTraits::GhostType::Mimic, "Мимик"},
    {GhostTraits::GhostType::Moroi, "Морой"},
    {GhostTraits::GhostType::Deogen, "Деоген"},
    {GhostTraits::GhostType::Thaye, "Тайе"}
};

const std::map<GhostAI::State, const char*> GhostStateNames = {
    {GhostAI::State::idle, "Idle"},
    {GhostAI::State::wander, "Wander"},
    {GhostAI::State::hunting, "Hunting"},
    {GhostAI::State::favouriteRoom, "Favourite Room"},
    {GhostAI::State::light, "Light"},
    {GhostAI::State::door, "Door"},
    {GhostAI::State::throwing, "Throwing"},
    {GhostAI::State::fusebox, "Fusebox"},
    {GhostAI::State::appear, "Appear"},
    {GhostAI::State::doorKnock, "Door Knock"},
    {GhostAI::State::windowKnock, "Window Knock"},
    {GhostAI::State::carAlarm, "Car Alarm"},
    {GhostAI::State::flicker, "Flicker"},
    {GhostAI::State::cctv, "CCTV"},
    {GhostAI::State::randomEvent, "Random Event"},
    {GhostAI::State::GhostAbility, "Ghost Ability"},
    {GhostAI::State::mannequin, "Mannequin"},
    {GhostAI::State::teleportObject, "Teleport Object"},
    {GhostAI::State::interact, "Interact"},
    {GhostAI::State::summoningCircle, "Summoning Circle"},
    {GhostAI::State::musicBox, "Music Box"},
    {GhostAI::State::dots, "Dots"},
    {GhostAI::State::salt, "Salt"}
};

const std::map<EMFGhostActionEv, const char*> EMFGhostActionNames = {
    {EMFGhostActionEv::GhostInteraction, "Ghost Interaction"},
    {EMFGhostActionEv::GhostThrowing, "Ghost Throwing"},
    {EMFGhostActionEv::GhostAppeared, "Ghost Appeared"},
    {EMFGhostActionEv::GhostEvidence, "Ghost Evidence - DO NOT USE"}
};

const std::map<EMFGhostActionType, const char*> EMFGhostActionTypeNames = {
    {EMFGhostActionType::interaction, "interaction"},
    {EMFGhostActionType::objectMoved, "objectMoved"},
    {EMFGhostActionType::windowKnock, "windowKnock"},
    {EMFGhostActionType::midGhostWrite, "midGhostWrite"},
    {EMFGhostActionType::breakerSwitch, "breakerSwitch"},
    {EMFGhostActionType::planchette, "planchette"},
    {EMFGhostActionType::rockingChair, "rockingChair"},
    {EMFGhostActionType::generalIntEffect, "generalIntEffect"},
    {EMFGhostActionType::lightSwitched, "lightSwitched"},
    {EMFGhostActionType::soundEffect, "soundEffect"},
    {EMFGhostActionType::crucifixFire, "crucifixFire"}
};

// todo:
/*
#include "UnityResolve.hpp"

namespace UnityEngine {
    struct MonoBehaviour : UnityResolve::UnityType::Object {};
    struct GameObject : UnityResolve::UnityType::Object {};
    struct Transform : UnityResolve::UnityType::Object {};
    struct Vector3 {
        float x, y, z;
        Vector3() : x(0), y(0), z(0) {}
        Vector3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}
    };
    namespace AI {
        struct NavMeshAgent : UnityResolve::UnityType::Object {};
    }
}

namespace Photon::Pun {
    struct PhotonMessageInfo {
        // Заглушка для PhotonMessageInfo
    };
    struct MonoBehaviourPun : UnityEngine::MonoBehaviour {};
}

namespace Micosmo::SensorToolkit {
    struct LOSSensor : UnityResolve::UnityType::Object {};
}

struct GhostModel;           // Предварительное объявление
struct GhostInfo;            // Предварительное объявление
struct GhostAudio;           // Предварительное объявление
struct GhostInteraction;     // Предварительное объявление
struct GhostActivity;        // Предварительное объявление
struct Player;               // Предварительное объявление
struct PhotonObjectInteract; // Предварительное объявление
struct WhiteSage;            // Предварительное объявление

struct GhostAI : Photon::Pun::MonoBehaviourPun {
    // Перечисление состояний
    enum class State {
        idle,
        wander,
        hunting,
        favouriteRoom,
        light,
        door,
        throwing,
        fusebox,
        appear,
        doorKnock,
        windowKnock,
        carAlarm,
        flicker,
        cctv,
        randomEvent,
        GhostAbility,
        mannequin,
        teleportObject,
        interact,
        summoningCircle,
        musicBox,
        dots,
        salt,
        ignite
    };

    // Поля
    void* levelController;                       // Offset: 0x28 (readonly, предположительно LevelController)
    State currentState;                          // Offset: 0x30
    GhostInfo* ghostInfo;                        // Offset: 0x38
    UnityEngine::AI::NavMeshAgent* navMeshAgent; // Offset: 0x40
    GhostAudio* ghostAudio;                      // Offset: 0x48
    GhostInteraction* ghostInteraction;          // Offset: 0x50
    GhostActivity* ghostActivity;                // Offset: 0x58
    GhostModel* currentGhostModel;               // Offset: 0x60 [HideInInspector]
    GhostModel* halloweenModel;                  // Offset: 0x68 [SerializeField]
    GhostModel* holidayModel;                    // Offset: 0x70 [SerializeField]
    GhostModel* easterModel;                     // Offset: 0x78 [SerializeField]
    GhostModel** normalModels;                   // Offset: 0x80 (массив)
    GhostModel** specialModels;                  // Offset: 0x88 (массив)
    bool isActive;                               // Offset: 0x90
    UnityEngine::Rendering::ShadowCastingMode shadowMode; // Offset: 0x94 [HideInInspector]
    std::vector<UnityEngine::Vector3> waypoints; // Offset: 0x98 [HideInInspector]
    float wanderTimer;                           // Offset: 0xA0
    Micosmo::SensorToolkit::LOSSensor* losSensor;// Offset: 0xA8
    bool isHunting;                              // Offset: 0xB0 [HideInInspector]
    UnityEngine::Transform* headTransform;       // Offset: 0xB8
    UnityEngine::Transform* bodyTransform;       // Offset: 0xC0
    UnityEngine::Transform* feetTransform;       // Offset: 0xC8
    float huntTimer;                             // Offset: 0xD0 [HideInInspector]
    float interactionCooldown;                   // Offset: 0xD4 [HideInInspector]
    float visibilityRange;                       // Offset: 0xD8 [HideInInspector]
    bool canInteract;                            // Offset: 0xDC [HideInInspector]
    bool canFlash;                               // Offset: 0xDD [HideInInspector]
    UnityEngine::Vector3 lastKnownPosition;      // Offset: 0xE0 [HideInInspector]
    UnityEngine::GameObject* ghostObject;        // Offset: 0xF0
    bool isVisible;                              // Offset: 0xF8 [HideInInspector]
    bool isChasing;                              // Offset: 0xF9 [HideInInspector]
    bool isTeleporting;                          // Offset: 0xFA [HideInInspector]
    bool isBlocked;                              // Offset: 0xFB [HideInInspector]
    WhiteSage* whiteSage;                        // Offset: 0x100 [HideInInspector]
    float chaseDuration;                         // Offset: 0x108
    bool isPaused;                               // Offset: 0x10C [HideInInspector]
    bool isStunned;                              // Offset: 0x10D [HideInInspector]
    bool isAggressive;                           // Offset: 0x10E [HideInInspector]
    Player* targetPlayer;                        // Offset: 0x110 [HideInInspector]
    int huntCount;                               // Offset: 0x118 [HideInInspector]
    UnityEngine::Vector3 targetPosition;         // Offset: 0x11C [HideInInspector]
    float* wanderSpeeds;                         // Offset: 0x128 (readonly массив)
    float* chaseSpeeds;                          // Offset: 0x130 (readonly массив)
    float* patrolTimes;                          // Offset: 0x138 (readonly массив)
    int patrolIndex;                             // Offset: 0x140
    int waypointIndex;                           // Offset: 0x144
    int eventCounter;                            // Offset: 0x148
    int flashCounter;                            // Offset: 0x14C
    int chaseTargetIndex;                        // Offset: 0x150
    float* eventDelays;                          // Offset: 0x158 (readonly массив)
    float* actionTimes;                          // Offset: 0x160 (readonly массив)
    float* huntDurations;                        // Offset: 0x168 (readonly массив)
    float stunTimer;                             // Offset: 0x170
    int* normalModelIndices;                     // Offset: 0x178 (readonly массив)
    int* specialModelIndices;                    // Offset: 0x180 (readonly массив)
    int* huntIndices;                            // Offset: 0x188 (readonly массив)
    int* eventIndices;                           // Offset: 0x190 (readonly массив)

    // Конструктор
    GhostAI() {
        static UnityResolve::UnityType::Method* method = nullptr;
        if (!method) {
            method = UnityResolve::Get("Assembly-CSharp.dll")
                         ->Get("GhostAI")
                         ->Get<UnityResolve::UnityType::Method>("<.ctor>");
        }
        if (method) {
            method->Invoke<void>(this);
        }
    }

    // Виртуальные методы MonoBehaviour
    virtual void Awake() override {
        static UnityResolve::UnityType::Method* method = nullptr;
        if (!method) {
            method = UnityResolve::Get("Assembly-CSharp.dll")
                         ->Get("GhostAI")
                         ->Get<UnityResolve::UnityType::Method>("Awake");
        }
        if (method) {
            method->Invoke<void>(this);
        }
    }

    virtual void Start() override {
        static UnityResolve::UnityType::Method* method = nullptr;
        if (!method) {
            method = UnityResolve::Get("Assembly-CSharp.dll")
                         ->Get("GhostAI")
                         ->Get<UnityResolve::UnityType::Method>("Start");
        }
        if (method) {
            method->Invoke<void>(this);
        }
    }

    virtual void Update() override {
        static UnityResolve::UnityType::Method* method = nullptr;
        if (!method) {
            method = UnityResolve::Get("Assembly-CSharp.dll")
                         ->Get("GhostAI")
                         ->Get<UnityResolve::UnityType::Method>("Update");
        }
        if (method) {
            method->Invoke<void>(this);
        }
    }

    // Публичные методы
    void Init(void* levelController) {
        static UnityResolve::UnityType::Method* method = nullptr;
        if (!method) {
            method = UnityResolve::Get("Assembly-CSharp.dll")
                         ->Get("GhostAI")
                         ->Get<UnityResolve::UnityType::Method>("Init");
        }
        if (method) {
            method->Invoke<void>(this, levelController);
        }
    }

    void SetGhostModel(GhostModel* model, bool force = false) {
        static UnityResolve::UnityType::Method* method = nullptr;
        if (!method) {
            method = UnityResolve::Get("Assembly-CSharp.dll")
                         ->Get("GhostAI")
                         ->Get<UnityResolve::UnityType::Method>("SetGhostModel");
        }
        if (method) {
            method->Invoke<void>(this, model, force);
        }
    }

    void ChangeState(State state, PhotonObjectInteract* interact = nullptr, bool immediate = false) {
        static UnityResolve::UnityType::Method* method = nullptr;
        if (!method) {
            method = UnityResolve::Get("Assembly-CSharp.dll")
                         ->Get("GhostAI")
                         ->Get<UnityResolve::UnityType::Method>("ChangeState");
        }
        if (method) {
            method->Invoke<void>(this, state, interact, immediate);
        }
    }

    void Appear(int delay = -1) {
        static UnityResolve::UnityType::Method* method = nullptr;
        if (!method) {
            method = UnityResolve::Get("Assembly-CSharp.dll")
                         ->Get("GhostAI")
                         ->Get<UnityResolve::UnityType::Method>("Appear");
        }
        if (method) {
            method->Invoke<void>(this, delay);
        }
    }

    void UnAppear() {
        static UnityResolve::UnityType::Method* method = nullptr;
        if (!method) {
            method = UnityResolve::Get("Assembly-CSharp.dll")
                         ->Get("GhostAI")
                         ->Get<UnityResolve::UnityType::Method>("UnAppear");
        }
        if (method) {
            method->Invoke<void>(this);
        }
    }

    void FlashAppear() {
        static UnityResolve::UnityType::Method* method = nullptr;
        if (!method) {
            method = UnityResolve::Get("Assembly-CSharp.dll")
                         ->Get("GhostAI")
                         ->Get<UnityResolve::UnityType::Method>("FlashAppear");
        }
        if (method) {
            method->Invoke<void>(this);
        }
    }

    void ForceFlash(bool enable) {
        static UnityResolve::UnityType::Method* method = nullptr;
        if (!method) {
            method = UnityResolve::Get("Assembly-CSharp.dll")
                         ->Get("GhostAI")
                         ->Get<UnityResolve::UnityType::Method>("ForceFlash");
        }
        if (method) {
            method->Invoke<void>(this, enable);
        }
    }

    void LookAtPlayer(Player* player) {
        static UnityResolve::UnityType::Method* method = nullptr;
        if (!method) {
            method = UnityResolve::Get("Assembly-CSharp.dll")
                         ->Get("GhostAI")
                         ->Get<UnityResolve::UnityType::Method>("LookAtPlayer");
        }
        if (method) {
            method->Invoke<void>(this, player);
        }
    }

    bool LookAtNearestPlayer() {
        static UnityResolve::UnityType::Method* method = nullptr;
        if (!method) {
            method = UnityResolve::Get("Assembly-CSharp.dll")
                         ->Get("GhostAI")
                         ->Get<UnityResolve::UnityType::Method>("LookAtNearestPlayer");
        }
        if (method) {
            return method->Invoke<bool>(this);
        }
        return false;
    }

    bool GetNearestPlayer(Player** outPlayer) {
        static UnityResolve::UnityType::Method* method = nullptr;
        if (!method) {
            method = UnityResolve::Get("Assembly-CSharp.dll")
                         ->Get("GhostAI")
                         ->Get<UnityResolve::UnityType::Method>("GetNearestPlayer");
        }
        if (method) {
            return method->Invoke<bool>(this, outPlayer);
        }
        return false;
    }

    void TemporarilyStopWander(float duration) {
        static UnityResolve::UnityType::Method* method = nullptr;
        if (!method) {
            method = UnityResolve::Get("Assembly-CSharp.dll")
                         ->Get("GhostAI")
                         ->Get<UnityResolve::UnityType::Method>("TemporarilyStopWander");
        }
        if (method) {
            method->Invoke<void>(this, duration);
        }
    }

    void RemoveInteractionBlock(float duration) {
        static UnityResolve::UnityType::Method* method = nullptr;
        if (!method) {
            method = UnityResolve::Get("Assembly-CSharp.dll")
                         ->Get("GhostAI")
                         ->Get<UnityResolve::UnityType::Method>("RemoveInteractionBlock");
        }
        if (method) {
            method->Invoke<void>(this, duration);
        }
    }

    void StartGhost() {
        static UnityResolve::UnityType::Method* method = nullptr;
        if (!method) {
            method = UnityResolve::Get("Assembly-CSharp.dll")
                         ->Get("GhostAI")
                         ->Get<UnityResolve::UnityType::Method>("StartGhost");
        }
        if (method) {
            method->Invoke<void>(this);
        }
    }

    // Корoutines (возвращают IEnumerator как void* для простоты)
    void* StopGhostFromHunting() {
        static UnityResolve::UnityType::Method* method = nullptr;
        if (!method) {
            method = UnityResolve::Get("Assembly-CSharp.dll")
                         ->Get("GhostAI")
                         ->Get<UnityResolve::UnityType::Method>("StopGhostFromHunting");
        }
        if (method) {
            return method->Invoke<void*>(this);
        }
        return nullptr;
    }

    void* StartHuntingTimer() {
        static UnityResolve::UnityType::Method* method = nullptr;
        if (!method) {
            method = UnityResolve::Get("Assembly-CSharp.dll")
                         ->Get("GhostAI")
                         ->Get<UnityResolve::UnityType::Method>("StartHuntingTimer");
        }
        if (method) {
            return method->Invoke<void*>(this);
        }
        return nullptr;
    }

    void* TrapWish(float duration, bool condition) {
        static UnityResolve::UnityType::Method* method = nullptr;
        if (!method) {
            method = UnityResolve::Get("Assembly-CSharp.dll")
                         ->Get("GhostAI")
                         ->Get<UnityResolve::UnityType::Method>("TrapWish");
        }
        if (method) {
            return method->Invoke<void*>(this, duration, condition);
        }
        return nullptr;
    }

    // RPC методы с Photon
    void MakeGhostAppear(bool visible, int index, Photon::Pun::PhotonMessageInfo info) {
        static UnityResolve::UnityType::Method* method = nullptr;
        if (!method) {
            method = UnityResolve::Get("Assembly-CSharp.dll")
                         ->Get("GhostAI")
                         ->Get<UnityResolve::UnityType::Method>("MakeGhostAppear");
        }
        if (method) {
            method->Invoke<void>(this, visible, index, info);
        }
    }

    void Hunting(bool active, int targetIndex, Photon::Pun::PhotonMessageInfo info) {
        static UnityResolve::UnityType::Method* method = nullptr;
        if (!method) {
            method = UnityResolve::Get("Assembly-CSharp.dll")
                         ->Get("GhostAI")
                         ->Get<UnityResolve::UnityType::Method>("Hunting");
        }
        if (method) {
            method->Invoke<void>(this, active, targetIndex, info);
        }
    }

    void SetCanFlash(bool canFlash, Photon::Pun::PhotonMessageInfo info) {
        static UnityResolve::UnityType::Method* method = nullptr;
        if (!method) {
            method = UnityResolve::Get("Assembly-CSharp.dll")
                         ->Get("GhostAI")
                         ->Get<UnityResolve::UnityType::Method>("SetCanFlash");
        }
        if (method) {
            method->Invoke<void>(this, canFlash, info);
        }
    }

    // Приватные методы (заглушки)
private:
    bool CheckLineOfSight(UnityEngine::Vector3 position, UnityEngine::Vector3* outHit, float range) {
        static UnityResolve::UnityType::Method* method = nullptr;
        if (!method) {
            method = UnityResolve::Get("Assembly-CSharp.dll")
                         ->Get("GhostAI")
                         ->Get<UnityResolve::UnityType::Method>("CheckLineOfSight"); // Примерное имя
        }
        if (method) {
            return method->Invoke<bool>(this, position, outHit, range);
        }
        return false;
    }

    void UpdateGhostState() {
        static UnityResolve::UnityType::Method* method = nullptr;
        if (!method) {
            method = UnityResolve::Get("Assembly-CSharp.dll")
                         ->Get("GhostAI")
                         ->Get<UnityResolve::UnityType::Method>("UpdateGhostState"); // Примерное имя
        }
        if (method) {
            method->Invoke<void>(this);
        }
    }
};

// Пример использования
int main() {
    UnityResolve::Init(GetModuleHandle(NULL), UnityResolve::Mode::Il2Cpp);

    GhostAI* ghost = new GhostAI();
    ghost->Awake();
    ghost->Start();
    ghost->Update();

    ghost->Init(nullptr);
    ghost->SetGhostModel(nullptr);
    ghost->ChangeState(GhostAI::State::wander);

    delete ghost;
    return 0;
}
*/