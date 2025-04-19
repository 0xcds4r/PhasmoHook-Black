#pragma once

class MultiplayerController : public II::MonoBehaviour
{
public:
	II::Camera* sceneCamera;
	II::List<II::Transform*>* spawns;
};
VALIDATE_SIZE(MultiplayerController, 0x20 + STRUCT_STUCK);