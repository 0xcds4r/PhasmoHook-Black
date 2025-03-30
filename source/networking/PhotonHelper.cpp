#include "../main.h"

void* PhotonHelper::GetLocalPlayer()
{
    return (void*)INVOKE_METHOD_EA(uintptr_t, "PhotonUnityNetworking.dll", "PhotonNetwork", "get_LocalPlayer");
}
