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


	ACTION_STATE _actionState;				//�s�����

	class WeaponBase* _weapon;				//����

	class CameraComponent* _cameraCom;		//�J����
	class MotionComponent* _motCom;			//���[�V����	
	class CapsuleComponent* _capsule;		//�J�v�Z��
	class AnimationComponent* _anim;		//�A�j���[�V����
	class LightsOutComponent* _lightsOut;	//���C�g�A�E�g

	float _moveSpeedMag;					//�ړ����x�{��	

	int _decoyTimes;						//�f�R�C�̎c���

	class SpriteText* _decoyTimesTextBg;	//�f�R�C�̎c��񐔂̔w�i
	class SpriteNumber* _decoyTimesText;	//�f�R�C�̎c��񐔂�UI�e�L�X�g
};

