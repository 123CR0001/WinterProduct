#pragma once
#include"Component.h"

class CreateAfterImageComponent :public Component {
public:
	CreateAfterImageComponent(class AnimationComponent* owner);
	~CreateAfterImageComponent();

	bool Process()override;

private:

	const int _maxCnt;					//�c���𐶐�����Ԋu

	int _frameCnt;						//�c���𐶐�����Ԋu�̃J�E���g

	class AnimationComponent* _anim;	//�A�j���[�V�����R���|�[�l���g
};