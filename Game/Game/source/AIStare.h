#pragma once
#include"AIState.h"

class AIStare :public AIState {
public:
	AIStare(class AIComponent* owner);
	~AIStare();

	const char* GetName()override { return "Stare"; }

	void OnEnter()override;
	void OnExist()override;

	bool Process()override;
};