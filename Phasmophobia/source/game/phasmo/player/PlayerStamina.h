#pragma once

// 0.12.1.0 [+]

class Player;
class PlayerStamina : II::MonoBehaviour {
public:
    void* outOfStaminaSource;       // Offset: 0x20
    void** outOfBreathMale;          // Offset: 0x28 
    void** outOfBreathFemale;        // Offset: 0x30 
    Player* player;                                    // Offset: 0x38
    bool maleSoundsOverride;                           // Offset: 0x40
    bool femaleSoundsOverride;                         // Offset: 0x41
    bool isMoving;                // Offset: 0x42
    bool canSprint;                    // Offset: 0x43
    bool isSprinting;                    // Offset: 0x44
    bool isWalking;                        // Offset: 0x45
    bool draining;                    // Offset: 0x46
    bool canDrainStamina;                        // Offset: 0x47 - *
    bool newIsSprinting;                        // Offset: 0x48
    bool newIsWalking;                        // Offset: 0x49
    bool staminaDrained;                        // Offset: 0x4A
    bool delayed;                        // Offset: 0x4B
    float currentStaminaRecharge;                        // Offset: 0x4C - maybe stamina counter
    float currentStamina;                        // Offset: 0x50 -- * sets every 3.0 for antistamina
    float restoreDelay;                        // Offset: 0x54
    void* OnStaminaDrained;        // Offset: 0x58
    void* OnStaminaRestored;      // Offset: 0x60
public:
    float GetCurrentStamina() {
        return this->currentStamina;
    }

    void SetCurrentStamina(float stamina) {
        this->currentStamina = stamina;
    }

    void PreventStaminaDrainForTime(float time) {
        static UnityResolve::Method* method;
        if (!method) {
            method = UnityResolve::Get("Assembly-CSharp.dll")
                ->Get("PlayerStamina")
                ->Get<UnityResolve::Method>("PreventStaminaDrainForTime");
        }
        if (method) {
            method->Invoke<void>(this, time);
        }
    }
};
VALIDATE_SIZE(PlayerStamina, 0x58 + STRUCT_STUCK);