#pragma once
#include"AIState.h"
#include<vector>
#include"../../../AppFrame/source/Collision/Vector3D.h"

//�v���C���[�����������|�C���g�Ɉړ�����
class AIChasePlayer:public AIState{
public:
	AIChasePlayer(class AIComponent* owner);
	~AIChasePlayer();

	bool Process()override;

	void OnEnter()override;

	void OnExist()override;

	void GetShortestRoots();

	const char* GetName()override { return "ChasePlayer"; }

	int GetPointsNum()const { return _pointsNum; }

private:

	int _pointsNum;

	//
	int _frameCnt;

};