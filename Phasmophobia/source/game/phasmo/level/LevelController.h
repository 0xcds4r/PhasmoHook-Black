#pragma once

// 0.12.1.0 [+]

struct SmallMapVariation {
    II::String* name;
    II::List<void*>* doorsToDisable;
    II::List<void*>* cctv;
    II::List<LevelRoom*>* rooms;
    II::List<II::GameObject*>* objectsToEnable;
    II::List<II::GameObject*>* objectsToDisable;
    II::List<II::Transform*>* fuseboxSpawns;
    II::List<void*>* blockedZones;
};
VALIDATE_SIZE(SmallMapVariation, 0x40);

class GhostAI;
class LevelController : public II::MonoBehaviourPun {
public:
    static inline LevelController* instance;
public:
    II::List<SmallMapVariation>* smallMapVariations;
    LevelRoom* currentPlayerRoom;
    LevelRoom* currentGhostRoom;
    GhostAI* currentGhost;
    II::List<void*>* doors;
    II::List<LevelRoom*>* allGhostRooms;
    II::List<LevelRoom*>* allInteriorRooms;
    II::List<II::Transform*>* fuseboxSpawnLocations;
    II::List<II::Transform*>* MannequinTeleportSpots;
    LevelRoom* outsideRoom;
    bool isOutsideMap;
    void* fuseBox;
    GameController* gameController;
    void* soundController;
    void* itemSpawner;
    II::List<void*>* exitDoors;
    II::List<void*>* levelAreas;
    II::List<void*>* crucifix;
    II::List<void*>* fireSources;
    II::List<void*>* allEquipment;
    II::List<II::String*>* possibleMaleFirstNames;
    II::List<II::String*>* possibleFemaleFirstNames;
    II::List<II::String*>* possibleLastNames;
    void* mainDoorKey;
    II::List<void*>* allBlockedZones;
    int intermediateZonesBlocked;
    int proZonesBlocked;
    int nightmareZonesBlocked;
    II::List<void*>* allClothDoors;
    II::List<float>* fps;
};
VALIDATE_SIZE(LevelController, 0x100 + STRUCT_STUCK);