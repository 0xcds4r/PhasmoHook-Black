#pragma once

class MapController : II::MonoBehaviour {
public:
    struct Floor {
        LevelRoom::FloorType floorType; // Offset: 0x10
        II::Transform* floor;  // Offset: 0x18
    };

    void* playerIcons;
    II::List<Player*>* players;
    Floor* allFloors;
    int currentFloorIndex; // Offset: 0x38 
    float iconScale; // Offset: 0x3C
    float playerIconScale; // Offset: 0x40
    void* motionSensorData; // Offset: 0x48
    GameController* gameController; // Offset: 0x50
    LevelRoom::FloorType defaultFloor; // Offset: 0x58 
    II::Vector3 playerIconOffset; // Offset: 0x5C
    II::Camera* mapCamera; // Offset: 0x68

public:
    auto GetPlayers() {
        return MapController::GetInstance()->players->ToArray()->ToVector();
    }
    
    int GetPlayersCount() 
    {
        if (GetPlayers().empty()) return 0;
        int count = 0;
        for (auto player : GetPlayers()) {
            if(player) 
                count++;
        }
        return count;
    }

    static inline MapController* instance = nullptr;
    static MapController* GetInstance() {
        return instance;
    }
};
VALIDATE_SIZE(MapController, 0x60 + STRUCT_STUCK);