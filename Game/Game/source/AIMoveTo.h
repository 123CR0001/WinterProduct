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
	int _pointsNum;				//次に向かう座標の番号
	Vector3 _oldCheckPoint;		//前回の目的地
	int _interval;				//ステートを変える間隔
};