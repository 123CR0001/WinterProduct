#pragma once
#include"Component.h"
#include"appframe.h"

class MoveComponent :public Component {
public:
	MoveComponent(class ObjectBase* owner, int order = 100);
	~MoveComponent();

	bool Process()override;

	void SetMoveSpeed(const float& set) { _moveSpeed = set; }
	void SetRotateSpeed(const float& set) { _rotateSpeed = set; }

	float GetSpeed()const;
	Vector3 GetRatateSpeed()const;
	
private:

	float _rotateSpeed;
	float _moveSpeed;

	Vector3 _oldPos;
	Vector3 _oldEulerAngle;
};