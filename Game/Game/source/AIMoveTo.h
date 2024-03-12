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

	const char* GetName()override { return "AIMoveTo"; }
private:
	int _pointsNum;
	Vector3D _oldCheckPoint;
	int _frameCnt;
};