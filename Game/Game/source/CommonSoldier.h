#pragma once
#include"CharaBase.h"


class CommonSoldier : public CharaBase{
public:
	CommonSoldier(class ObjectServer* server);
	virtual ~CommonSoldier();

	bool Initialize()override;
	bool Terminate()override;
	bool Process()override;
	bool Render()override;

	void SetJsonDataUE(nlohmann::json data)override;

	float GetDetectionLevel()const { return _detectionLevel; }

	class AIComponent* GetAIComponent()const { return _AI; }

	bool GetAttack()const { return _isAttack; }
protected:

	float _detectionLevel;				//���m�x

	class AIComponent* _AI;				//AI�R���|�[�l���g
	class CapsuleComponent* _capsule;	//�J�v�Z���R���|�[�l���g
	class AnimationComponent* _anim;	//�A�j���[�V�����R���|�[�l���g

	//�U�����邩
	bool _isAttack;

	class Gun* _gun;					//�e(����I�u�W�F�N�g)
};