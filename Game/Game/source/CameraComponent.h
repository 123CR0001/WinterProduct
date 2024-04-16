#pragma once
#include"Component.h"
#include"appframe.h"

class CameraComponent :public Component {
public:
	CameraComponent(class ObjectBase* owner,int oder = 9999);
	virtual ~CameraComponent();

	virtual bool Initialize() = 0;
	bool Process()override;

	Vector3 GetPos()const { return _pos; }		//カメラの位置を取得
	Vector3 GetAngle()const { return _angle; }	//カメラの角度を取得

	//カメラを揺らす
	void Swap() { _swapWidth = rand() % 30 + 30; _swapHeight = rand() % 30 + 30; }

	//ターゲットとの距離の倍率を設定
	void SetTargetDistmag(float mag) { _targetDistMag = mag; }

	//ターゲットとの距離の倍率を取得
	const float GetTargetDistMag()const { return _targetDistMag; }
protected:
	Vector3 _pos;		//カメラの位置
	Vector3 _angle;		//カメラの角度


	float _targetDistMag;		//ターゲットとの距離の倍率

	int _swapWidth, _swapHeight;		//揺れ幅
};