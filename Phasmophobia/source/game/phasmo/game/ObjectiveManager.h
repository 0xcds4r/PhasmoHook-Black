#pragma once
#include <vector>

enum class ObjectiveType : std::int32_t
{
	BlowoutCandle,
	DetectParabolicSound,
	EMFGhost,
	EscapeHunt,
	GhostPhoto,
	IdentifyGhost,
	MotionSensorGhost,
	PreventHunt,
	SanityBelow25,
	SmudgeGhost,
	SmudgeHunt,
	WitnessEvent,
	DnaEvidence
};

struct ObjectiveValues : public II::GameObject
{
	ObjectiveType type;
	II::String* localisedKey;
	bool requiresMultiplayer;
	int32_t requiredLevel;
	int32_t moneyReward;
};
VALIDATE_SIZE(ObjectiveValues, 0x28 + STRUCT_STUCK);

struct __declspec(align(8)) Objective
{
	ObjectiveValues* values;
	II::String* name;
	bool completed;
	int32_t objectiveID;
};
VALIDATE_SIZE(Objective, 0x18);

class ObjectiveManager : public II::MonoBehaviourPun
{
public:
	static inline ObjectiveManager* instance = nullptr;
public:
	ObjectiveValues* smallMap;
	ObjectiveValues* mediumMap;
	ObjectiveValues* largeMap;
	ObjectiveValues* smallMapBone;
	ObjectiveValues* mediumMapBone;
	ObjectiveValues* largeMapBone;
	II::List<ObjectiveValues>* allSideObjectiveValues;
	ObjectiveValues* selectedMainObjectiveValue;
	ObjectiveValues* selectedBoneObjectiveValue;
	II::List<ObjectiveValues*>* availableSideObjectives;
	void* titleText;
	void* respondsToText;
	void* mapImage;
	II::List<void*>* allObjectiveTexts;
	II::List<void*>* allObjectiveImages;
	II::List<void*>* allObjectivePhotos;
	II::List<void*>* allObjectiveString;
	bool hasCreatedObjectives;
	int32_t missionIndex;
	bool _HasSetupObjectives_k__BackingField;
	int32_t interactionVariation;
	int32_t deadVariation;
	int32_t crucifixVariation;
	II::List<int*>* missionsToSet;

	void CompleteObjective(ObjectiveType type) {
		static UnityResolve::Method* method;
		if (!method) {
			method = UnityResolve::Get("Assembly-CSharp.dll")
				->Get("ObjectiveManager")
				->Get<UnityResolve::Method>("CompleteObjective");
		}
		if (method) {
			method->Invoke<void>(this, type);
		}
	}

	void CompleteAllObjectives() 
	{
		Sleep(2000);
		this->CompleteObjective(ObjectiveType::BlowoutCandle);
		this->CompleteObjective(ObjectiveType::DetectParabolicSound);
		this->CompleteObjective(ObjectiveType::EMFGhost);
		this->CompleteObjective(ObjectiveType::EscapeHunt);
		this->CompleteObjective(ObjectiveType::GhostPhoto);
		this->CompleteObjective(ObjectiveType::IdentifyGhost);
		this->CompleteObjective(ObjectiveType::MotionSensorGhost);
		this->CompleteObjective(ObjectiveType::PreventHunt);
		this->CompleteObjective(ObjectiveType::SanityBelow25);
		this->CompleteObjective(ObjectiveType::SmudgeGhost);
		this->CompleteObjective(ObjectiveType::SmudgeHunt);
		this->CompleteObjective(ObjectiveType::WitnessEvent);
		this->CompleteObjective(ObjectiveType::DnaEvidence);
	}
};
VALIDATE_SIZE(ObjectiveManager, 0xC0 + STRUCT_STUCK);