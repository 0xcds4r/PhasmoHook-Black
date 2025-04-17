#pragma once

class FirstPersonController : II::MonoBehaviour {
public:
    bool CanMove;
    bool CanTurn;
    bool IsSprinting;
    bool IsMoving;
    bool canSprint;
    float m_RunstepLenghten;                            // Offset: 0x28 [SerializeField, Range(0f, 1f)]
    float m_StickToGroundForce;                         // Offset: 0x2C [SerializeField]
    float m_GravityMultiplier;                          // Offset: 0x30 [SerializeField]
    void* m_MouseLook; //UnityStandardAssets::Utility::MouseLook m_MouseLook;// Offset: 0x38
    bool m_UseFOVKick;                                 // Offset: 0x40
    void* m_FOVKick;//UnityStandardAssets::Utility::FOVKick m_FOVKick;    // Offset: 0x48
    bool m_EnableAudio;                                 // Offset: 0x50
    bool m_PreviouslyGrounded;                          // Offset: 0x51
    void* mHeadBob;//UnityStandardAssets::Utility::CurveControlledBob m_HeadBob; // Offset: 0x58
    float m_StepInterval;                               // Offset: 0x60 [SerializeField]
    II::Camera* m_Camera;                      // Offset: 0x68 [SerializeField]
    II::Vector2 m_Input;                       // Offset: 0x70
    II::Vector3 m_MoveDir;                     // Offset: 0x78
    PhysicsCharacterController* m_CharacterController;  // Offset: 0x88
    bool m_Jump;                                        // Offset: 0x90 [HideInInspector]
    float m_fSpeed;                                  // Offset: 0x94
    II::Vector3 m_OriginalCameraPosition;      // Offset: 0x98
    II::RaycastHit m_HitInfo;                  // Offset: 0xA4
    void* m_LandEvent;                // Offset: 0xD0
    void* m_JumpEvent;                // Offset: 0xD8
};
VALIDATE_SIZE(FirstPersonController, 0xD0); // really valid?