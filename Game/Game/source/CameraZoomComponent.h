#pragma once
#include"Component.h"

class CameraZoomComponent :public Component {
public:
	CameraZoomComponent(class CameraComponent* component,float changeMag,int maxFrameCnt);
	~CameraZoomComponent();

	bool Process()override;

private:

	//どの倍率にするか
	float _changeMag;

	//変更前の値
	float _mag;

	//何フレームでズームが終わるか
	int _maxFrameCnt;
	//何フレーム経ったか記録する
	int _frameCnt;

	//ズームをするカメラ
	class CameraComponent* _camera;
};