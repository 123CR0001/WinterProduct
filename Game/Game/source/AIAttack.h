#pragma once
#include"AIState.h"

class AIAttack :public AIState {
public:
	AIAttack(class AIComponent* owner);
	~AIAttack();

	void OnEnter()override;
	void OnExist()override;
	bool Process()override;

private:
	int _frameCnt;
};