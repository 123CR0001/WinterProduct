#pragma once
#include"CharaBase.h"

class Player :public CharaBase {
public:
	Player(class ObjectServer* server);
	~Player();

	bool Initialize()override;
	bool Terminate()override;
	bool Process()override;
	bool Render()override;

	bool ChangeState(std::string)override;

	void AddMoveSpeedMag(const float up) { _moveSpeedMag += up; _moveSpeedMag = Clamp(1.f, 2.f, _moveSpeedMag); }

	class CapsuleComponent* GetCapsuleComponent() { return _capsule; }

	enum class ACTION_STATE :unsigned int{
		kIdle,
		kWalk,
		kAttack,
		kAttack2,
		kSilent,
		kSilentWalk
	};

	class CameraComponent* GetCamera()const { return _cameraCom; }
	void SetCamera(class CameraComponent* camera) { _cameraCom = camera; }
protected:


	ACTION_STATE _actionState;

	class WeaponBase* _weapon;

	class CameraComponent* _cameraCom;
	class MotionComponent* _motCom;
	class CapsuleComponent* _capsule;

	float _moveSpeedMag;
};

