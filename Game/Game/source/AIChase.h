#pragma once
#include"AIState.h"
#include<vector>
#include"../../../AppFrame/source/Collision/Vector3D.h"

//�v���C���[�����������|�C���g�Ɉړ�����
class AIChase:public AIState{
public:
	AIChase(class AIComponent* owner);
	~AIChase();

	bool Process()override;

	void OnEnter()override;

	void OnExist()override;

	void GetShortestRoots();

	const char* GetName()override { return "Chase"; }

	int GetPointsNum()const { return _pointsNum; }

private:

	int _pointsNum;

};