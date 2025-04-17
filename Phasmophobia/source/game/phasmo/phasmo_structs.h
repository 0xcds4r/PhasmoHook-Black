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
    void* pEvidence;
    int field1;
    int field2;
    EMFGhostActionEv ghostAction;
    EMFGhostActionType ghostActionType;
    float field3;
};
VALIDATE_SIZE(EMF, 0x30 + STRUCT_STUCK);

class DNAEvidence : public II::MonoBehaviour {
public:
    void* pData;
};

class EvidenceController : public II::MonoBehaviour {
public:
    void* skip1;
    void* skip2;
    void* skip3;
    DNAEvidence* pDNAEvidence;
    void* ghostOrb;
    void* skip4;
    void* skip5;
    bool unknownBool1;
    float unknownFlt1;
    void* skip6;
    II::Color unkColor;
};
VALIDATE_SIZE(EvidenceController, 0x68 + STRUCT_STUCK);

#include "item/CursedItemsController.h"
