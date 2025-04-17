#pragma once

class PlayerCharacter : public II::MonoBehaviourPun {
public:
	bool specialState;
	II::MeshRenderer* pMeshRender;
	II::GameObject* pGameObject;
	II::Transform* pTransform;
	void* _LegacyBadge;
	void* _PrestigeBadge;
};
VALIDATE_SIZE(PlayerCharacter, 0x48 + STRUCT_STUCK);