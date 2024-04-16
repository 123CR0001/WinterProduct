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
	int _pointsNum;				//���Ɍ��������W�̔ԍ�
	Vector3 _oldCheckPoint;		//�O��̖ړI�n
	int _interval;				//�X�e�[�g��ς���Ԋu
};