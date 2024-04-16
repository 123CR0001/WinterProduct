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

	float _detectionLevel;				//検知度

	class AIComponent* _AI;				//AIコンポーネント
	class CapsuleComponent* _capsule;	//カプセルコンポーネント
	class AnimationComponent* _anim;	//アニメーションコンポーネント

	//攻撃するか
	bool _isAttack;

	class Gun* _gun;					//銃(武器オブジェクト)
};