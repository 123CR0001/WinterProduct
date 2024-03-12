#pragma once
#include"AIState.h"

class AIDiscovery :public AIState {
public:
	AIDiscovery(class AIComponent* ower);
	~AIDiscovery();

	const char* GetName() { return "Discovery"; }

	void OnEnter()override;
	void OnExist()override;

	bool Process()override;

private:

	int _frameCnt;
};