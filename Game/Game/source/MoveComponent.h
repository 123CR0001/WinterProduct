#pragma once
#include"Component.h"

class MoveComponent :public Component {
public:
	MoveComponent(class ObjectBase* owner, int order = 100);
	~MoveComponent();

	bool Process()override;

	void SetMoveSpeed(const float& set) { _moveSpeed = set; }
	void SetRotateSpeed(const float& set) { _rotateSpeed = set; }
private:

	float _rotateSpeed;
	float _moveSpeed;
};