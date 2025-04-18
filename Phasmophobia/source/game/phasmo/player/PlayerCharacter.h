#pragma once

// 0.12.1.0 [+]

class PlayerCharacter : public II::MonoBehaviourPun {
public:
	bool isMaleCharacter;
	II::MeshRenderer* vrRadio;
	II::GameObject* headphones;
	II::Transform* headAttach;
	void* legacyBadge;
	void* prestigeBadge;
};
VALIDATE_SIZE(PlayerCharacter, 0x48 + STRUCT_STUCK);