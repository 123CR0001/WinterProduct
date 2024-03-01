#pragma once
#include"AIState.h"
#include"appframe.h"
#include<memory>

class AIStay :public AIState {
public:
	AIStay(class AIComponent* owner);
	~AIStay();

	void OnEnter()override;
	void OnExist()override;

	const char* GetName()override { return "Stay"; }

	bool Process()override;

private:
	std::unique_ptr<Vector3D>_ownerEulerAngle;
};