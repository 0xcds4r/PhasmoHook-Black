#pragma once

// 0.12.1.0 [+]

class __declspec(align(8)) MouseLook {
    float XSensitivity;
    float YSensitivity;
    bool clampVerticalRotation;
    float MinimumX;
    float MaximumX;
    bool smooth;
    float smoothTime;
    bool lockCursor;
    II::Quaternion m_CharacterTargetRot;
    II::Quaternion m_CameraTargetRot;
    bool m_cursorIsLocked;
    float horizontalLook;
    float verticalLook;
};
VALIDATE_SIZE(MouseLook, 0x50); // sizeof=0x50

class __declspec(align(8)) FOVKick {
    II::Camera* Camera;
    float FOVIncrease;
    float TimeToIncrease;
    float TimeToDecrease;
    bool fovUp;
};
VALIDATE_SIZE(FOVKick, 0x18); // sizeof=0x18

struct __declspec(align(8)) AnimationCurve
{
    intptr_t m_Ptr;
};
VALIDATE_SIZE(AnimationCurve, 0x8);

class __declspec(align(8)) CurveControlledBob {
    float HorizontalBobRange;
    float VerticalBobRange;
    AnimationCurve* Bobcurve;
    float VerticaltoHorizontalRatio;
    float m_CyclePositionX;
    float m_CyclePositionY;
    float m_BobBaseInterval;
    II::Vector3 m_OriginalCameraPosition;
    float m_Time;
    II::Transform* camTransform;
};
VALIDATE_SIZE(CurveControlledBob, 0x38);

class FirstPersonController : II::MonoBehaviour {
public:
    bool canMove;
    bool canTurn;
    bool m_IsSprinting;
    bool m_IsMoving;
    bool canSprint;
    float m_RunstepLenghten;                            // Offset: 0x28 [SerializeField, Range(0f, 1f)]
    float m_StickToGroundForce;                         // Offset: 0x2C [SerializeField]
    float m_GravityMultiplier;                          // Offset: 0x30 [SerializeField]
    MouseLook* m_MouseLook; //UnityStandardAssets::Utility::MouseLook m_MouseLook;// Offset: 0x38
    bool m_UseFovKick;                                 // Offset: 0x40
    FOVKick* m_FovKick;//UnityStandardAssets::Utility::FOVKick m_FOVKick;    // Offset: 0x48
    bool m_UseHeadBob;                                 // Offset: 0x50
    bool _toggleSprint;                          // Offset: 0x51
    CurveControlledBob* m_HeadBob;//UnityStandardAssets::Utility::CurveControlledBob m_HeadBob; // Offset: 0x58
    float m_StepInterval;                               // Offset: 0x60 [SerializeField]
    II::Camera* m_Camera;                      // Offset: 0x68 [SerializeField]
    II::Vector2 m_Input;                       // Offset: 0x70
    II::Vector3 m_MoveDir;                     // Offset: 0x78
    PhysicsCharacterController* m_CharacterController;  // Offset: 0x88
    bool m_Crouching;                                        // Offset: 0x90 [HideInInspector]
    float currentSpeed;                                  // Offset: 0x94
    II::Vector3 desiredMove;      // Offset: 0x98
    II::RaycastHit hitInfo;                  // Offset: 0xA4
    void* OnStartSprint;                // Offset: 0xD0
    void* OnStartWalk;                // Offset: 0xD8
};
VALIDATE_SIZE(FirstPersonController, 0xD0); // really valid?