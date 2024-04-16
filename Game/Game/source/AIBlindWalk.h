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
	int _rotateSpeed;				//回転速度
	int _rotateAngle;				//回転角度		
	float _moveDist;				//移動距離	
	float _moveSpeed;				//移動速度
	int _waitCnt;					//待機カウント

	float length;					//移動距離
};