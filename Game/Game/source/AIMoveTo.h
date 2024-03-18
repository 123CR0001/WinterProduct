#pragma once
#include"AIState.h"
#include"appframe.h"
class AIMoveTo :public AIState {
public:
	AIMoveTo(class AIComponent* owner);
	~AIMoveTo();

	bool Process()override;

	void OnEnter()override;
	void OnExist()override;

	const char* GetName()override { return "MoveTo"; }
private:
	int _pointsNum;
	Vector3 _oldCheckPoint;
	int _frameCnt;
};