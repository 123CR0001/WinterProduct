#pragma once
#include"ObjectBase.h"

class AfterImage :public ObjectBase {
public:
	AfterImage(class AnimationComponent* anim);
	~AfterImage();

	bool Process()override;
	bool Render()override;

private:

	//モデルのハンドル
	int _handle;

	//透明度
	float _alpha;
	int error;
};