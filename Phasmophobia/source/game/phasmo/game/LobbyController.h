#pragma once

class LobbyController : public II::MonoBehaviourPun
{
public:
	void* serverManager;
	void* cachedRoomList;
	void* roomOptions;
	II::Vector3 vrPlayerPosition;
	II::Quaternion vrPlayerRotation;
	II::Vector3 pcPlayerPosition;
	II::Quaternion pcPlayerRotation;
	bool joinOrCreateRoomInProcess;
};
VALIDATE_SIZE(LobbyController, 0x70 + STRUCT_STUCK);