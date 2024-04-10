#pragma once
#include"Animation.h"
#include<functional>

//SpriteNumber�̐������C�[�W���O����������
//���U���g�ȂǂŎg�p����
class EasingNumber :public Animation {
public:
	EasingNumber(class SpriteNumber* sprite, int frame, std::function<float(float, float, float, float)>func = EasingInQuad);
	~EasingNumber();

	bool Process()override;

private:
	//�g�p����C�[�W���O�̊֐�
	std::function<float(float ,float ,float ,float)>_func;

	//�`�悷��e�L�X�g
	class SpriteNumber* _spriteNumber;

	//�`�悷�鐔�l
	int _number;
};