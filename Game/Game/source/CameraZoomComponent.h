#pragma once
#include"Component.h"

class CameraZoomComponent :public Component {
public:
	CameraZoomComponent(class CameraComponent* component,float mag,int frameCnt);
	~CameraZoomComponent();

	bool Process()override;

private:

	//どの倍率にするか
	const float _changeMag;

	//変更前の値
	const float _mag;
	

	//何フレームでズームが終わるか
	const int _maxFrameCnt;
	//何フレーム経ったか記録する
	int _frameCnt;

	//ズームをするカメラ
	class CameraComponent* _camera;
};