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

	float _detectionLevel;	//åüímìx

	class AIComponent* _AI;
	class CapsuleComponent* _capsule;
	class AnimationComponent* _anim;

	//çUåÇÇ∑ÇÈÇ©
	bool _isAttack;

	class Gun* _gun;
};