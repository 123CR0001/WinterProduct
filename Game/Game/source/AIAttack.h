#pragma once
#include"AIState.h"


class AIAttack :public AIState {
public:
	AIAttack(class AIComponent* owner);
	~AIAttack();

	const char* GetName()override { return "Attack"; }

	void OnEnter()override;
	void OnExist()override;
	bool Process()override;

private:
	float _rotateAngle;			//‰ñ“]Šp“x
};