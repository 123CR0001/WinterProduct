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
	Vector3 GetRotateSpeed()const;
	
private:

	float _rotateSpeed;			//��]���x
	float _moveSpeed;			//�ړ����x		

	Vector3 _oldPos;			//�O�t���[���̍��W
	Vector3 _oldEulerAngle;		//�O�t���[���̊p�x
};