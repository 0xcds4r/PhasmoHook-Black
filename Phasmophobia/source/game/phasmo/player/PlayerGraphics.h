#pragma once

class Player;
class PlayerGraphics : II::MonoBehaviour {
public:
    Player* player;                                    
    void* volume;
    void* deadProfile;
    void* menuProfile;
    void* gameProfile;
    void* nightVisionProfile;
    void* ambientOcclusion;
    void* customProfile;
};
VALIDATE_SIZE(PlayerGraphics, 0x50 + STRUCT_STUCK);

//II::Volume* volume;                       
    //II::VolumeProfile* deadProfile;           // Offset: 0x30
    //II::VolumeProfile* menuProfile;           // Offset: 0x38
    //II::VolumeProfile* gameProfile;           // Offset: 0x40
    //II::VolumeProfile* nightVisionProfile;    // Offset: 0x48
    //II::ScriptableRendererFeature* ambientOcclusion; // Offset: 0x50
    //II::VolumeProfile* customProfile;