#pragma once

#include "level/ClosetZone.h"
#include "physics/PhysicsCharacterController.h"
#include "controller/FirstPersonController.h"
#include "level/LevelRoom.h"

// todo 

class SceneManager : public II::MonoBehaviour {
public:
    void* data;
};

class GameController : public II::MonoBehaviour {
public:
    void* data;
};

class RewardManager : public II::MonoBehaviour {
public:
    void* data;
};

class MainManager : public II::MonoBehaviour {
public:
    void* data;
};

class JournalController : public II::MonoBehaviour {
public:
    void* data;
};

#include "level/LevelController.h"

#include "player/Player.h"
#include "level/MapController.h"

#include "ghost/GhostAI.h"

#include "network/Network.h"

class EMF : public II::MonoBehaviour {
public:
    void* emfEvidence;
    int strength;
    int strengthOffset;
    EMFGhostActionEv ghostAction; // int32_t type;
    EMFGhostActionType ghostActionType; // int32_t emfInteractionType;
    float timerUntilDeath;
};
VALIDATE_SIZE(EMF, 0x30 + STRUCT_STUCK);

class DNAEvidence : public II::MonoBehaviourPun {
public:
    void* photonInteract;
    II::Transform* losTarget;
    II::Rigidbody* rigid;
    void* meshFilter;
    void* meshCollider;
    LevelRoom* levelRoom;
    II::List<II::Mesh*>* boneMeshes;
    void* collectable;
};
VALIDATE_SIZE(DNAEvidence, 0x58 + STRUCT_STUCK);

class EvidenceController : public II::MonoBehaviour {
public:
    II::List<void*>* evidenceInLevel;
    II::List<LevelRoom*>* roomsToSpawnDNAEvidenceInside;
    void* photonView;
    DNAEvidence* bone;
    II::Transform* ghostOrb;
    void* ghostOrbRenderer;
    LevelController* levelController;
    bool isFreezingTemperatureGhost;
    float dotsCheckTimer;
    II::List<void*>* activeFingerprints;
    II::Color foggyOrbColor;
};
VALIDATE_SIZE(EvidenceController, 0x68 + STRUCT_STUCK);

#include "item/CursedItemsController.h"
