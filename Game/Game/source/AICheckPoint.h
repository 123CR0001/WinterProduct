#pragma once
#include"AIState.h"

class AICheckPoint :public AIState {
public:
	AICheckPoint(class AIComponent* owner);
	~AICheckPoint();

	bool Process()override;

	void OnEnter()override;
	void OnExist()override;

	const char* GetName()override { return "CheckPoint"; }
private:
	int _pointsNum;

};