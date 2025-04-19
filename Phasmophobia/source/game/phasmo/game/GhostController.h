#pragma once

struct ThayeAge
{
	int age;
	int huntSanityRequirement;
	float huntSpeed;
	float interactionRateModifier;
	float ghostEventRateModifier;
};
VALIDATE_SIZE(ThayeAge, 0x14);

class GhostController : public II::MonoBehaviour
{
public:
	void* view;
	GhostTraits ghostTraits;
	bool createdGhost;
	void* ghostEventPlayer;
	LevelController* levelController;
	GameController* gameController;
	void* setupPhaseController;
	GhostAI* ghost;
	void* camoMaterial;
	void* dotsMaterial;
	int ghostID;
	bool canChangeGhostState;
	float ghostTypeStateChangeTimer;
	ThayeAge thayeAgeStats;
};
VALIDATE_SIZE(GhostController, 0xB8 + STRUCT_STUCK);