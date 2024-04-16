#pragma once
#include"AIState.h"

class AILookAround :public AIState {
public:
	AILookAround(class AIComponent* owner);
	~AILookAround();

	const char* GetName()override { return "LookAround"; }

	void OnEnter()override;
	void OnExist()override;

	bool Process()override;

private:
	int _frameCnt;		//���[�V�������I���܂ł̎���
};