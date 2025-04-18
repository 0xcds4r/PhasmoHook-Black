#pragma once

#include "PlayerCharacter.h"
#include "PlayerGraphics.h"
#include "PlayerStamina.h"

class Player : public II::MonoBehaviour {
public:
	void* photonView; 
	bool isDead; 
	bool isDyingLocally;
	int modelID;
	PlayerCharacter* playerCharacter;
	ClosetZone* closet;
	II::GameObject* headObject;
	II::Transform* trackedHead;
	void* keys;
	II::Camera* cam;
	LevelRoom* currentRoom;
	II::Transform* mapIcon;
	void* rightCurrentHeldObject;
	void* leftCurrentHeldObject;
	II::GameObject** characterModels;
	II::GameObject* ghostDeathHands;
	void* ghostRaycastMask;
	void* headSocket;
	void* losTargets;
	II::Transform* aiTargetPoint;
	II::GameObject* deadBodyPrefab;
	void* deadPlayer;
	void* playerSanity;
	void* playerStats;
	void* footstepController;
	void* journalController;
	II::Renderer** allRends;
	bool isSafeGhostCursed;
	void* playerAudio;
	PlayerGraphics* playerGraphics;
	void* playerSensors;
	PlayerStamina* stamina;
	float movementSpeed;
	bool monkeyPawLeaveEffectIsActive;
	PhysicsCharacterController* charController;
	void* listener;
	FirstPersonController* firstPersonController;
	void* pcPropGrab;
	void* dragRigidBodyUse;
	void* pcCanvas;
	void* pcCrouch;
	void* pcMenu;
	void* pcControls;
	void* pcFlashlight;
	II::Animator* charAnim;
	void* playerInput;
	void* itemSway;
	void* lightSway;
	II::Transform* deadPlayerSpawnPoint;
	void* leftHandInteractor;
	void* rightHandInteractor;
	II::Transform* xrOrigin;
	void* movementSettings;
	void* vrBelt;
	void* leftVRBeltDropZone;
	void* rightVRBeltDropZone;
	void* trailerCamera;
	void* vrLoading;
public:
	II::Animator* GetAnimator() {
        return charAnim;
	}

    PlayerCharacter* GetPlayerCharacter() {
        return playerCharacter;
    }

    PlayerGraphics* GetPlayerGraphics() {
        return playerGraphics;
    }

    PlayerStamina* GetPlayerStamina() {
        return stamina;
    }

    FirstPersonController* GetFirstPersonController() {
        return firstPersonController;
    }

    PhysicsCharacterController* GetPhysicsCharacterController() {
        return charController;
    }

    II::Camera* GetMainCamera() {
        return cam;
    }

    II::Transform* GetPlayerTransform() {
        return this->GetTransform();
    }

    II::GameObject* GetPlayerGameObject() {
        return this->GetGameObject();
    }

    II::Renderer** GetAllRends() {
		return allRends;
    }

    void* GetAudioListener() {
		return listener;
    }

    void* GetJournalController() {
		return journalController;
    }

    bool IsPlayerDead() {
        return isDead;
    }

    LevelRoom* GetCurrentRoom() {
		return currentRoom;
    }

    void* GetPlayerInput() {
		return playerInput;
    }
};
VALIDATE_SIZE(Player, 0x1C8 + STRUCT_STUCK);
// 0.12.1.0 :: 0x1C0 -> 0x1C8 [+]
