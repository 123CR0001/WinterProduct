#pragma once
#include"AIState.h"

class AIBlindWalk :public AIState {
public:
	AIBlindWalk(class AIComponent* owner);
	~AIBlindWalk();

	void OnEnter()override;
	void OnExist()override;
	bool Process()override;

	const char* GetName() { return "BlindWalk"; }

protected:
	int _rotateSpeed;				//��]���x
	int _rotateAngle;				//��]�p�x		
	float _moveDist;				//�ړ�����	
	float _moveSpeed;				//�ړ����x
	int _waitCnt;					//�ҋ@�J�E���g

	float length;					//�ړ�����
};