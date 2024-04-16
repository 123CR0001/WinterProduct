#pragma once
#include"AIState.h"
#include"AppFrame.h"
#include<vector>

class AIPatrol :public AIState {
public:
	AIPatrol(class AIComponent* owner);
	~AIPatrol();

	bool Process()override;

	void OnEnter()override;
	void OnExist()override;

	const char* GetName()override { return "Patrol"; }

	int GetPointsNum()const { return _pointsNum; }

private:
	int _pointsNum;		//Ÿ‚ÉŒü‚©‚¤À•W‚Ì”Ô†
};