#pragma once
#include"CharaBase.h"
#include<memory>

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
	class MotionComponent* GetMotionComponent()const { return _motCom; }
	class LightsOutComponent* GetLightsOutComponent()const { return _lightsOut; }

	enum class ACTION_STATE :unsigned int{
		kNone,
		kIdle,
		kWalk,
		kAttack,
		kAttack2,
		kAttack3,
		kSilent,
		kSilentWalk,
		kDeath,
		kClear
	};

	class CameraComponent* GetCamera()const { return _cameraCom; }
	void SetCamera(class CameraComponent* camera) { _cameraCom = camera; }

	void SetDecoyTimes(int times) { _decoyTimes = times; }

	ACTION_STATE GetState()const { return _actionState; }
protected:


	ACTION_STATE _actionState;				//行動状態

	class WeaponBase* _weapon;				//武器

	class CameraComponent* _cameraCom;		//カメラ
	class MotionComponent* _motCom;			//モーション	
	class CapsuleComponent* _capsule;		//カプセル
	class AnimationComponent* _anim;		//アニメーション
	class LightsOutComponent* _lightsOut;	//ライトアウト

	float _moveSpeedMag;					//移動速度倍率	

	int _decoyTimes;						//デコイの残り回数

	class SpriteText* _decoyTimesTextBg;	//デコイの残り回数の背景
	class SpriteNumber* _decoyTimesText;	//デコイの残り回数のUIテキスト
};

