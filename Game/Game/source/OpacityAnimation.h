#pragma once
#include"Animation.h"

class OpacityAnimation :public Animation{
public:
	OpacityAnimation(class SpriteText* text, int frame,float startAlpha,float endAlpha);
	OpacityAnimation(class SpriteText* text, int frame, float endAlpha);
	~OpacityAnimation();

	bool Process()override;
private:
	//�ŏ��̃A���t�@�l
	const float _startAlpha;

	//�ڎw���A���t�@�l
	const float _endAlpha;
};