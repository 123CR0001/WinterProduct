#pragma once
#include"AIState.h"

class AILoseSight :public AIState {
public:
	AILoseSight(class AIComponent* owner);
	~AILoseSight();

	const char* GetName()override { return "LoseSight"; }

	void OnEnter()override;
	void OnExist()override;

	bool Process()override;

private:
	int _frameCnt;		//ƒ‚[ƒVƒ‡ƒ“‚ªI‚í‚é‚Ü‚Å‚ÌŠÔ
};