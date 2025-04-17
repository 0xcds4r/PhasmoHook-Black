#pragma once

class ClosetZone : public II::MonoBehaviour {
public:
    enum class DoorOpenType {
        Open,
        Close
    };

    DoorOpenType doorOpenType;                         
    void* leftClosetDoor_;             
    void* rightClosetDoor_;             
    void* internalAreaObstacle_; 
    II::Transform* standOutsideEntrancePoint_; 
    II::Transform* navSpot_;              
    bool isClosetOccupied_ = false;                    
    void** playersInside_;           
};