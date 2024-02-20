#pragma once
#include"Component.h"

class CreateAfterImageComponent :public Component {
public:
	CreateAfterImageComponent(class AnimationComponent* owner);
	~CreateAfterImageComponent();

	bool Process()override;

private:
	//�c���𐶐�����Ԋu
	const int _maxCnt;

	//�t���[�����J�E���g����ϐ�
	int _frameCnt;

	class AnimationComponent* _anim;
};