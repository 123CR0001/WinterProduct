#pragma once
#include"AIState.h"
#include<vector>
#include"../../../AppFrame/source/Collision/Vector3D.h"

//プレイヤーを見かけたポイントに移動する
class AIChasePlayer:public AIState{
public:
	AIChasePlayer(class AIComponent* owner,class AIBackPatrol* AI);
	~AIChasePlayer();

	bool Process()override;

	void OnEnter()override;

	void OnExist()override;

	bool GetShortestRoots();

	const char* GetName()override { return "ChasePlayer"; }

	int GetPointsNum()const { return _pointsNum; }

private:

	int _pointsNum;

	//
	int _frameCnt;

	class AIBackPatrol* _AIBackPatrol;
};