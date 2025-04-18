#pragma once

// now is correct
// 0.12.1.0 [+]

class PhysicsCharacterController : public II::MonoBehaviour 
{
public:
	II::RaycastHit _groundCheckHitInfo;
	II::Vector3 _surfaceNormal;
	float _slopeAngle;
	bool _isSurfaceWalkable;
	II::Vector3 _inputVelocity;
	bool _hasNewInput;
	float _fallSpeed;
	II::LayerMask _rigidBodyLayerMask;
	int playerLayer;
	II::List<II::RaycastHit*>* _sweepHits;
	II::List<II::Collider*>* _overlapCapsuleColliders;
	II::RaycastHit _penetrationHitInfo;
	II::Vector3 _p1;
	II::Vector3 _p2;
	II::Rigidbody* _rigidBody;
	II::CapsuleCollider* _collider;
	int _maxIterationCount;
	int _numPenetrationProbes;
	float _slopeLimit_k__BackingField;
	II::LayerMask _groundLayers_k__BackingField;
	float _skinWidth_k__BackingField;
	float _groundCheckDistance_k__BackingField;
	float _stickToGroundDistance_k__BackingField;
	float minMoveDistance;
	float _gravityMultiplier_k__BackingField;
	bool _canBlockDoors_k__BackingField;
	II::Vector3 _velocity_k__BackingField;
	bool _isGrounded_k__BackingField;

	//char data[272 - 0x10 * 3];
	//II::Rigidbody* _rigidBody;                     // Offset: 0xD8 [SerializeField]
	//II::CapsuleCollider* _collider;                // Offset: 0xE0 [SerializeField]
	//int _maxIterationCount;                                 // Offset: 0xE8 [SerializeField, Min(1f)]
	//int _numPenetrationProbes;                              // Offset: 0xEC [SerializeField, Min(1f)]
	//float minMoveDistance;                                  // Offset: 0x104 [SerializeField, Min(0f)]
public:
	auto SetEnabled(bool value) -> void {
		static UnityResolve::Method* method;
		if (!method) method = UnityResolve::Get("Assembly-CSharp.dll")->Get("PhysicsCharacterController")->Get<UnityResolve::Method>("set_enabled");
		if (method) method->Invoke<void>(this, value);
	}

	auto GetRigidbody() -> II::Rigidbody* {
		static UnityResolve::Method* method;
		if (!method) method = UnityResolve::Get("Assembly-CSharp.dll")->Get("PhysicsCharacterController")->Get<UnityResolve::Method>("get_Rigidbody"); // Или "GetRigidbody", если есть нормальное имя
		if (method) return method->Invoke<II::Rigidbody*>(this);
		return nullptr;
	}

	auto GetCapsuleCollider() -> II::CapsuleCollider* {
		static UnityResolve::Method* method;
		if (!method) method = UnityResolve::Get("Assembly-CSharp.dll")->Get("PhysicsCharacterController")->Get<UnityResolve::Method>("get_CapsuleCollider"); // Или "GetCapsuleCollider"
		if (method) return method->Invoke<II::CapsuleCollider*>(this);
		return nullptr;
	}

	auto Move(UnityResolve::UnityType::Vector3 motion) -> void {
		static UnityResolve::Method* method;
		if (!method) method = UnityResolve::Get("Assembly-CSharp.dll")->Get("PhysicsCharacterController")->Get<UnityResolve::Method>("Move");
		if (method) method->Invoke<void>(this, motion);
	}
};
VALIDATE_SIZE(PhysicsCharacterController, 0x110 + STRUCT_STUCK);