#pragma once
#include"CameraComponent.h"

class FollowCamera : public CameraComponent {
public:
	FollowCamera(class ObjectBase* owner, int order = 9999);
	virtual ~FollowCamera();

	bool Initialize()override;
	bool Process()override;

protected:

	float _springConst;						//ばね定数

	//ターゲットとの直線的な距離ではなく、底辺と高さ
	float _widthDist;
	float _heightDist;

	Vector3 _velocity;							//ばね定数を用いた計算に必要な速度を保持するための変数
};