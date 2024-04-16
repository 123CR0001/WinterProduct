#pragma once
#include"AIState.h"
#include"AppFrame.h"

class AIBackPatrol :public AIState {
public:
	AIBackPatrol(class AIComponent* owner);
	~AIBackPatrol();

	bool Process()override;

	void OnEnter()override;
	void OnExist()override;

	const char* GetName()override { return "BackPatrol"; }

	int GetPointsNum()const { return _pointsNum; }

private:
	int _pointsNum;		//Ÿ‚ÉŒü‚©‚¤À•W‚Ì”Ô†	
};
