#pragma once
#include"Component.h"

class CameraZoomComponent :public Component {
public:
	CameraZoomComponent(class CameraComponent* component,float changeMag,int maxFrameCnt);
	~CameraZoomComponent();

	bool Process()override;

private:

	float _changeMag;						//変更する倍率
	float _mag;								//元の倍率

	int _maxFrameCnt;						//何フレームでズームするか
	int _frameCnt;							//何フレーム経ったか記録する

	class CameraComponent* _camera;			//ズームをするカメラ
};