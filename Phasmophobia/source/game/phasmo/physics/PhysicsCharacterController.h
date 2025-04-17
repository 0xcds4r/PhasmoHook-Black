#pragma once

class PhysicsCharacterController : public II::MonoBehaviour 
{
public:
	char data[272 - 0x10 * 3];
	II::Rigidbody* _rigidBody;                     // Offset: 0xD8 [SerializeField]
	II::CapsuleCollider* _collider;                // Offset: 0xE0 [SerializeField]
	int _maxIterationCount;                                 // Offset: 0xE8 [SerializeField, Min(1f)]
	int _numPenetrationProbes;                              // Offset: 0xEC [SerializeField, Min(1f)]
	float minMoveDistance;                                  // Offset: 0x104 [SerializeField, Min(0f)]
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