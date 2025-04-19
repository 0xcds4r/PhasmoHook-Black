#pragma once

class Evidence : public II::MonoBehaviourPun
{
    void* photoValue;
    bool hasAlreadyTakenPhoto;
    bool resetTakenPhotoOnEnable;
    void* OnPhotoTaken;
};
VALIDATE_SIZE(Evidence, 0x30 + STRUCT_STUCK);

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

class Fingerprint : public II::MonoBehaviourPun
{
    Evidence* evidence;
    II::MeshRenderer* fingerprintRenderer;
    II::List<void*>* normalFingerprintTextures;
    II::List<void*>* smallFingerprintTextures;
    II::List<void*>* multiSmallFingerprintTextures;
    int32_t fingerprintType;
    void* disablePrints;
};
VALIDATE_SIZE(Fingerprint, 0x50 + STRUCT_STUCK);

class EvidenceController : public II::MonoBehaviour {
public:
    II::List<Evidence*>* evidenceInLevel;
    II::List<LevelRoom*>* roomsToSpawnDNAEvidenceInside;
    void* photonView;
    DNAEvidence* bone;
    II::Transform* ghostOrb;
    void* ghostOrbRenderer;
    LevelController* levelController;
    bool isFreezingTemperatureGhost;
    float dotsCheckTimer;
    II::List<Fingerprint*>* activeFingerprints;
    II::Color foggyOrbColor;
};
VALIDATE_SIZE(EvidenceController, 0x68 + STRUCT_STUCK);