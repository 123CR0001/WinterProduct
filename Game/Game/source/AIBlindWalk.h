#pragma once
#include"AIState.h"

class AIBlindWalk :public AIState {
public:
	AIBlindWalk(class AIComponent* owner);
	~AIBlindWalk();

	void OnEnter()override;
	void OnExist()override;
	bool Process()override;

	const char* GetName() { return "BlindWalk"; }

protected:
	int _rotateSpeed;
	int _rotateAngle;
	float _moveDist;
	float _moveSpeed;
	int _waitCnt;
};