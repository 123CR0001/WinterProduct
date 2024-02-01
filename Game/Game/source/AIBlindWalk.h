#pragma once
#include"AIState.h"

class AIBlindWalk :public AIState {
public:
	AIBlindWalk(class AIComponent* owner);
	~AIBlindWalk();

	void OnEnter()override;
	void OnExist()override;
	bool Process()override;

	const char* GetName() { return "RandomWalk"; }

protected:
	float _rotateSpeed;
	float _rotateAngle;
	float _moveDist;
};