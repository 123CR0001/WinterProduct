#pragma once
#include"AIState.h"

class AIRandomWalk :public AIState {
public:
	AIRandomWalk(class AIComponent* owner);
	~AIRandomWalk();

	void OnEnter()override;
	void OnExist()override;
	bool Process()override;

	const char* GetName() { return "AIRandomWalk"; }

protected:
	float _rotateAngle;
	float _moveDist;
};