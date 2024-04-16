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

	std::function<float(float ,float ,float ,float)>_func;				//�g�p����C�[�W���O�̊֐�

	class SpriteNumber* _spriteNumber;									//�C�[�W���O����SpriteNumber	

	int _number;														//�C�[�W���O���鐔��	
};