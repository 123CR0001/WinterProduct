#pragma once
#include"AIState.h"
#include"appframe.h"

class AIRush :public AIState {
public:
	AIRush(class AIComponent* owner);
	~AIRush();

	void OnEnter()override;
	void OnExist()override;

	const char* GetName()override { return "Rush"; }

	bool Process()override;

private:
	Vector3 _rushPos;
};