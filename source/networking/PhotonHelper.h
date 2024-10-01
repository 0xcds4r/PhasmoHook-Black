#pragma once

class PhotonHelper
{
public:
    static void* GetPlayerPhotonView(Player* _this) 
    {
        void* photonView = GetOffsetValue<void*>((void*)_this, 0x20, "Assembly-CSharp.dll", "Player");
        return photonView;
    }

    static void* GetOwner(void* _photon) {
        void* owner = GetOffsetValue<void*>((void*)_photon, 0x80, "PhotonUnityNetworking.dll", "PhotonView");
        return owner;
    }

    static II::String* GetRPNickName(void* _realTimePlayer) 
    {
        II::String* rpNickName = GetOffsetValue<II::String*>((void*)_realTimePlayer, 0x20, "PhotonRealtime.dll", "Player");
        return rpNickName;
    }

    static bool IsRPLocalPlayer(void* _realTimePlayer) 
    {
        bool isLocalPlayer = GetOffsetValue<bool>((void*)_realTimePlayer, 0x1C, "PhotonRealtime.dll", "Player");
        return isLocalPlayer;
    }
};