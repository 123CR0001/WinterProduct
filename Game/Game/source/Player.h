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
	class AnimationComponent* GetAnimationComponent()const { return _anim; }

	enum class ACTION_STATE :unsigned int{
		kNone,
		kIdle,
		kWalk,
		kAttack,
		kAttack2,
		kSilent,
		kSilentWalk,
		kDead
	};

	class CameraComponent* GetCamera()const { return _cameraCom; }
	void SetCamera(class CameraComponent* camera) { _cameraCom = camera; }

	void SetIsLightsOut(bool isLightsOut) { _isLightsOut = isLightsOut; }

	ACTION_STATE GetState()const { return _actionState; }
protected:


	ACTION_STATE _actionState;

	class WeaponBase* _weapon;

	class CameraComponent* _cameraCom;
	class MotionComponent* _motCom;
	class CapsuleComponent* _capsule;
	class AnimationComponent* _anim;

	bool _isLightsOut;

	float _moveSpeedMag;
};

