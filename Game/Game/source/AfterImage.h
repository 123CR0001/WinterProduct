#pragma once
#include"ObjectBase.h"

class AfterImage :public ObjectBase {
public:
	AfterImage(class AnimationComponent* anim);
	~AfterImage();

	bool Process()override;
	bool Render()override;

private:

	//���f���̃n���h��
	int _handle;

	//�����x
	float _alpha;
	int error;
};