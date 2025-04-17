#pragma once

class Player;
class PlayerStamina : II::MonoBehaviour {
public:
    void* outOfStaminaSource;       // Offset: 0x20
    void** outOfBreathMale;          // Offset: 0x28 
    void** outOfBreathFemale;        // Offset: 0x30 
    Player* player;                                    // Offset: 0x38
    bool maleSoundsOverride;                           // Offset: 0x40
    bool femaleSoundsOverride;                         // Offset: 0x41
    bool field_1;                // Offset: 0x42
    bool field_2;                    // Offset: 0x43
    bool field_3;                    // Offset: 0x44
    bool field_4;                        // Offset: 0x45
    bool field_5;                    // Offset: 0x46
    bool field_6;                        // Offset: 0x47 - *
    bool field_7;                        // Offset: 0x48
    bool field_8;                        // Offset: 0x49
    bool field_9;                        // Offset: 0x4A
    bool field_10;                        // Offset: 0x4B
    float field_11;                        // Offset: 0x4C - maybe stamina counter
    float currentStamina;                        // Offset: 0x50 -- * sets every 3.0 for antistamina
    float field_13;                        // Offset: 0x54
    void* event_1;        // Offset: 0x58
    void* event_2;      // Offset: 0x60
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