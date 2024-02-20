#pragma once
#include"AIState.h"

class AITrase :public AIState {
public:
	AITrase(class AIComponent* owner);
	~AITrase();

	void OnEnter()override;
	void OnExist()override;

	const char* GetName()override { return "Trase"; }

	bool Process()override;
private:
	int _pointsNum;
};