#pragma once

#include "PlayerCharacter.h"
#include "PlayerGraphics.h"
#include "PlayerStamina.h"

class Player : public II::MonoBehaviour {
public:
	void* photonView; // 0x20
	bool isDead; // 0x28
	bool field2; // 0x29
	char byte1; // skip byte
	char byte2; // skip byte
	int field3;
	PlayerCharacter* playerCharacter;
	ClosetZone* closetZone;
	II::GameObject* pGameObject;
	II::Transform* pTransform;
	void* keyInfo;
	II::Camera* pCamera;
	LevelRoom* pRoom;
	II::Transform* pTransform2;
	void* photonObjInteract;
	void* photonObjInteract2;
	II::GameObject** gameObjects;
	II::GameObject* pGameObject2;
	void* layerMask;
	void* _socket;
	II::Transform* pTransform3;
	II::GameObject* pGameObject3;
	void* deadPlayer;
	void* playerSanity;
	void* playerStats;
	void* footstepController;
	void* journalController;
	II::Renderer** renders;
	bool field4;
	void* playerAudio;
	PlayerGraphics* playerGraphics;
	void* playerSensors;
	PlayerStamina* playerStamina;
	float field5;
	bool field6;
	PhysicsCharacterController* physicsCharacterController;
	void* audioListener;
	FirstPersonController* firstPersonController;
	void* _PCPropGrab;
	void* dragRigidbodyUse;
	void* _PCCanvas;
	void* _PCCrouch;
	void* _PCMenu;
	void* _PCControls;
	void* _PCFlashlight;
	II::Animator* animator;
	void* playerInput;
	void* _PCItemSway;
	void* _PCItemSway2;
	II::Transform* pTransform5;
	void* xrInteractor1;
	void* xrInteractor2;
	II::Transform* pTransform6;
	void* _VRMovementSettings;
	void* _VRBelt;
	void* _SocketInteractor1;
	void* _SocketInteractor2;
	void* _TrailerCamera;
	void* _VRLoading;
public:
	II::Animator* GetAnimator() {
        return animator;
	}

    PlayerCharacter* GetPlayerCharacter() {
        return playerCharacter;
    }

    PlayerGraphics* GetPlayerGraphics() {
        return playerGraphics;
    }

    PlayerStamina* GetPlayerStamina() {
        return playerStamina;
    }

    FirstPersonController* GetFirstPersonController() {
        return firstPersonController;
    }

    PhysicsCharacterController* GetPhysicsCharacterController() {
        return physicsCharacterController;
    }

    II::Camera* GetMainCamera() {
        return pCamera;
    }

    II::Transform* GetPlayerTransform() {
        return this->GetTransform();
    }

    II::GameObject* GetPlayerGameObject() {
        return this->GetGameObject();
    }

    II::Renderer** GetRenderer() {
		return renders;
    }

    void* GetAudioListener() {
		return audioListener;
    }

    void* GetJournalController() {
		return journalController;
    }

    bool IsPlayerDead() {
        return isDead;
    }

    LevelRoom* GetCurrentRoom() {
		return pRoom;
    }

    void* GetXRInteractor() {
		return xrInteractor1;
    }

    void* GetPlayerInput() {
		return playerInput;
    }
};
VALIDATE_SIZE(Player, 0x1C0 + STRUCT_STUCK);
