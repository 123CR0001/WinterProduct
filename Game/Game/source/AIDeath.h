#pragma once
#include"AIState.h"

class AIDeath :public AIState {
public:
	AIDeath(class AIComponent* owner);
	~AIDeath();

	const char* GetName()override { return "Death"; }

	void OnEnter()override;
	void OnExist()override;

	bool Process()override;

private:
	int _frameCnt;

};