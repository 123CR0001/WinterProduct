#pragma once
#include"Component.h"
#include"appframe.h"

class CameraComponent :public Component {
public:
	CameraComponent(class ObjectBase* owner,int oder = 9999);
	virtual ~CameraComponent();

	virtual bool Initialize() = 0;
	bool Process()override;

	Vector3 GetPos()const { return _pos; }	
	Vector3 GetAngle()const { return _angle; }
	void Swap() { _swapWidth = rand() % 30 + 30; _swapHeight = rand() % 30 + 30; }

	void SetTargetDistmag(const float mag) { _targetDistMag = mag; }
	const float GetTargetDistMag()const { return _targetDistMag; }
protected:
	Vector3 _pos;
	Vector3 _angle;

	//ターゲットとの距離の倍率
	float _targetDistMag;

	int _swapWidth, _swapHeight;
};