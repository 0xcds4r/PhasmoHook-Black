#pragma once

// 0.12.1.0 [+/-]

class Prop : public II::MonoBehaviour
{
    void* photonInteract;
    bool kinematicOnStart;
    bool kinematicOnImpact;
    bool hostOnlyPhysics;
    float requiredImpactForce;
    bool disableImpactSounds;
    bool canPlayImpactSound;
    II::List<void*>* impactClips;
    void* OnImpact;
    bool unparentOnAwake;
};
VALIDATE_SIZE(Prop, 0x40 + STRUCT_STUCK); // sizeof=0x40

class Equipment : public Prop
{
    void* storeItem;
    bool isPlaceable;
    bool isVRPlaceable;
    bool isPlaced;
    bool kinematicOnPlace;
    II::Vector3 placementAxis;
    II::Vector3 vrPlacementAxis;
    II::LayerMask placeableMask;
    float interactDistance;
    float placementDistance;
    bool opensWhenPlaced;
    //char pad_1[7];
    II::Transform* helperObject;
    II::Transform* openHelper;
    II::Transform* closedHelper;
    II::List<II::Renderer*>* helperObjectRends;
    II::Vector3 helperOffsetRot;
    II::Vector3 manualRotationOffset;
    bool isElectronic;
    bool isHeadMounted;
    bool canPlaceOnTripods;
    bool canRotate;
    //char pad_2[4];
    void* attachedTripod;
    bool socketPlacing;
    bool vrSocketPlacing;
    bool hasOnOffSounds;
    //char pad_3[5];
    void* noise;
    void* onSound;
    void* offSound;
    void* disturbanceSource;
    bool isConsumable;
    bool isOn;
    bool isBeingUsed;
    bool isPlacing;
    //char pad_4[3];
    void* delaySequence;
    bool canUse;
    float useDelay;
    LevelController* levelController;
    void* socket;
    II::Ray pcAim;
    II::RaycastHit hit;
    bool isNearActiveGhost;
    //char pad_5[3];
    II::String* ownerUnityID;
};
VALIDATE_SIZE(Equipment, 0x178); // sizeof=0x160

class CursedItem : public Equipment
{
    bool inUse;
    bool hasBroken;
    bool hasInitialised;
};
VALIDATE_SIZE(CursedItem, 0x180);  // sizeof=0x168

class OuijaBoard : public CursedItem {
    II::List<void*>* questions;
    II::Transform* marker;
    II::List<II::Transform*>* letterPositions;
    II::List<II::String*>* lettersList;
    II::Renderer* rend;
    void* evidence;
    II::Color glowColor;
    II::List<II::MeshRenderer*>* brokenPieces;
    void* breakParticles;
    void* breakSound;
    void* moveSound;
    void* ouijaSequence;
    II::String* purpose;
    II::String* causeOfDeath;
    II::String* feelings;
    bool isHideAndSeek;
    bool allowAnotherQuestion;
    void* pcVoiceRecognitionUI;
    void* vrVoiceRecognitionUI;
    void* voiceRecognitionUI;
    void* speechRecognitionController;
    bool isUsingTextVoiceLanguage;
    bool inLocalUse;
};
VALIDATE_SIZE(OuijaBoard, 0x230); // sizeof=0x218

class MusicBox : public CursedItem {
    II::Transform* lid;
    void* musicSource;
    void* clashSound;
    void* openSound;
    void* closeSound;
    II::Animator* anim;
    void* boxSequence;
    bool isOpen;
    bool ghostAppeared;
    float distConst;
    float maxDistance;
    float distanceFromPlayer;
    float distanceFromGhost;
};
VALIDATE_SIZE(MusicBox, 0x1D0); // sizeof=0x1B8

class TarotCards : public CursedItem {
    void* tarotEvidence;
    void* tarotCard;
    II::Animator* leftHandAnim;
    II::Animator* rightHandAnim;
    II::List<void>* cardDrawClips;
    II::List<II::GameObject*>* cardsInDeck;
    void* vrTarotCard;
    II::Collider* vrTarotCardCollider;
};
VALIDATE_SIZE(TarotCards, 0x1C0); // sizeof=0x1A8

class SummoningCircle : public CursedItem {
    void* ritualParticles;
    II::Transform* spawnPoint;
    II::List<void*>* allCandles;
    void* OnSummoningCircleUsed;
    void* ritualSequence;
    float insanityOfPlayerWhoLitCircle;
};
VALIDATE_SIZE(SummoningCircle, 0x1B0); // sizeof=0x198

class HauntedMirror : public CursedItem {
    II::Camera* mirrorCamera;
    II::Light* mirrorCamLight;
    II::MeshRenderer* mirrorMeshRend;
    II::MeshRenderer* frameMeshRend;
    II::Animator* anim;
    void* smashParticles;
    void* smashSound;
    bool drainSanity;
    float sanityDrained;
};
VALIDATE_SIZE(HauntedMirror, 0x1C0); // sizeof=0x1A8

class VoodooDoll : public CursedItem {
    void* heartPinSound;
    II::List<void*>* pinSounds;
    void* allPins;
    II::List<II::Rigidbody*>* allRigids;
    II::List<II::Collider*>* jointColliders;
    int32_t huntCount;
}; 
VALIDATE_SIZE(VoodooDoll, 0x1B0); // sizeof=0x198

class MonkeyPaw : public CursedItem {
    II::Animator* anim;
    II::List<void*>* crunchClips;
    II::List<void*>* twitchClips;
    II::GameObject* tagObj;
    void* muffleSnapshot;
    II::List<void*>* wishes;
    int32_t wishesLeft;
    bool isUsingTextVoiceLanguage;
    bool isTextInUse;
    void* pcVoiceRecognitionUI;
    void* vrVoiceRecognitionUI;
    void* voiceRecognitionUI;
    void* speechRecognitionController;
    bool isSeeGhostCursed;
    bool isKnowledgeGhostCursed;
    bool usedSeeGhost;
    bool usedActivity;
    bool usedSanity;
    bool usedTrap;
    bool usedRevive;
    bool usedSafe;
    bool usedLeave;
    bool usedKnowledge;
    bool usedWeather;
    LevelRoom* previousRoom;
    II::GameObject* postProcessingObject;
    void* seeGhostFadeSequence;
};
VALIDATE_SIZE(MonkeyPaw, 0x200); // sizeof=0x1E8

enum class CursedItemType : std::int32_t
{
    none,
    tarotCards,
    ouijaBoard,
    mirror,
    musicBox,
    summoningCircle,
    voodooDoll,
    monkeyPaw
};

class CursedItemsController : public II::MonoBehaviour
{
public:
    static inline CursedItemsController* instance;
public:
    OuijaBoard* ouijaBoard;
    MusicBox* musicBox;
    TarotCards* tarotCards;
    SummoningCircle* summoningCircle;
    HauntedMirror* hauntedMirror;
    VoodooDoll* voodooDoll;
    MonkeyPaw* monkeyPaw;
    II::Transform* ouijaBoardSpawnSpots;
    II::Transform* musicBoxSpawnSpots;
    II::Transform* tarotCardsSpawnSpots;
    II::Transform* summoningCircleSpawnSpots;
    II::Transform* hauntedMirrorSpawnSpots;
    II::Transform* voodooDollSpawnSpots;
    II::Transform* monkeyPawSpawnSpots;
    II::List<CursedItemType>* usedCursedItems;
};
VALIDATE_SIZE(CursedItemsController, 0x88 + STRUCT_STUCK);