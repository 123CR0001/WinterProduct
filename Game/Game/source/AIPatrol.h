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

	std::vector<Vector3D> GetPatrolPoints()const{ return _patrolPoints; }
	int GetPatrolPointsNum()const { return _patrolPointsNum; }

private:
	std::vector<Vector3D>_patrolPoints;
	int _patrolPointsNum;
};