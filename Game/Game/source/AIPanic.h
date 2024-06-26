#pragma once
#include"AIState.h"

class AIPanic :public AIState {
public:
	AIPanic(class AIComponent* owner);
	~AIPanic();

	void OnEnter()override;
	void OnExist()override;
	bool Process()override;

	const char* GetName()override { return "Panic"; }

private:
	int _frameCnt;		//モーションが終わるまでの時間
};