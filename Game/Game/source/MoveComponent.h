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

	float _rotateSpeed;			//回転速度
	float _moveSpeed;			//移動速度		

	Vector3 _oldPos;			//前フレームの座標
	Vector3 _oldEulerAngle;		//前フレームの角度
};