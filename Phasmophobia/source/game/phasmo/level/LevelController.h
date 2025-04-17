#pragma once

struct SmallMapVariation {
    II::String* Name;
    II::List<void*>* DoorsToDisable;
    II::List<void*>* CCTVs;
    II::List<LevelRoom*>* Rooms;
    II::List<II::GameObject*>* ObjectsToEnable;
    II::List<II::GameObject*>* ObjectsToDisable;
    II::List<II::Transform*>* FuseBoxSpawns;
    II::List<void*>* BlockedZones;
};
VALIDATE_SIZE(SmallMapVariation, 0x40);

class GhostAI;
class LevelController : public II::MonoBehaviourPun {
public:
    static inline LevelController* instance;
public:
    II::List<SmallMapVariation>* MapVariations;
    LevelRoom* CurrentRoom;
    LevelRoom* GhostRoom;
    GhostAI* _Ghost;
    II::List<void*>* Doors;
    II::List<LevelRoom*>* Rooms;
    II::List<LevelRoom*>* SecondaryRooms;
    II::List<II::Transform*>* SpawnPoints;
    II::List<II::Transform*>* GhostSpawnPoints;
    LevelRoom* StartingRoom;
    bool IsLevelActive;
    void* FuseBox;
    GameController* GameController;
    void* SoundController;
    void* ItemSpawner;
    II::List<void*>* SpecialDoors;
    II::List<void*>* LevelAreas;
    II::List<void*>* Crucifixes;
    II::List<void*>* FireSources;
    II::List<void*>* InteractableObjects;
    II::List<II::String*>* RoomNames;
    II::List<II::String*>* AreaNames;
    II::List<II::String*>* ObjectNames;
    void* Key;
    II::List<void*>* BlockedZones;
    int IntermediateZonesBlocked;
    int ProZonesBlocked;
    int NightmareZonesBlocked;
    II::List<void*>* ItemSpawnPoints;
    II::List<float>* RoomTemperatures;
};
VALIDATE_SIZE(LevelController, 0x100 + STRUCT_STUCK);