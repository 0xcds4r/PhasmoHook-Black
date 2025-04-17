#pragma once

class LevelRoom : public II::MonoBehaviour {
public:
    enum class RoomType { 
        insideRoom,
        outsideMap,
        exterior,
        exteriorRoom
    };

    enum class FloorType {
        basement,
        firstFloor,
        secondFloor,
        thirdFloor,
        fourthFloor,
        fifthFloor,
        sixthFloor,
        seventhFloor,
        eighthFloor,
        ninthFloor,
        tenthFloor
    };
public:
    II::List<void*>* lightSwitches; // Offset: 0x20  LightSwitch
    void** doors;                            // Offset: 0x28 
    II::List<II::Collider>* roomColliders; // Offset: 0x30 [SerializeField]
    II::Transform* centerTransform; // Offset: 0x38 
    void* reverbZone; // Offset: 0x40  // II::AudioReverbZone
    RoomType roomType;                       // Offset: 0x48 
    II::BoxCollider* roomBoxCollider; // Offset: 0x50
    FloorType floorType;                     // Offset: 0x58
    II::String* roomName;                    // Offset: 0x60
    float temperature;                       // Offset: 0x68 
    float lastTemperatureUpdate;             // Offset: 0x6C 
    float ambientTemperature;                // Offset: 0x70 
    float temperatureUpdateTimer;            // Offset: 0x74
    bool isBasementOrAttic;                  // Offset: 0x78 
    bool isOutdoorRoom;                      // Offset: 0x79
    bool isRoomActive;                       // Offset: 0x7A
    bool isTemperatureStable;                // Offset: 0x7B 
};
VALIDATE_SIZE(LevelRoom, 0x70 + STRUCT_STUCK);