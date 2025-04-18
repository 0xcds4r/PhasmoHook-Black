#pragma once

// 0.12.1.0 [+]

class ClosetZone : public II::MonoBehaviour {
public:
    enum class DoorOpenType {
        Open,
        Close
    };

    DoorOpenType doorOpenType;                         
    void* leftClosetDoor;
    void* rightClosetDoor;
    void* internalAreaObstacle;
    II::Transform* standOutsideEntrancePoint;
    II::Transform* navSpot;
    bool hasChecked;
    void** players;
};