#pragma once
#include"AIState.h"

class AITrace :public AIState {
public:
	AITrace(class AIComponent* owner);
	~AITrace();

	void OnEnter()override;
	void OnExist()override;

	const char* GetName()override { return "Trace"; }

	bool Process()override;
private:
	int _pointsNum;
};