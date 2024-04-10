#pragma once
#include"Animation.h"

class OpacityAnimation :public Animation{
public:
	OpacityAnimation(class SpriteText* text, int frame,float startAlpha,float endAlpha);
	OpacityAnimation(class SpriteText* text, int frame, float endAlpha);
	~OpacityAnimation();

	bool Process()override;
private:
	//最初のアルファ値
	const float _startAlpha;

	//目指すアルファ値
	const float _endAlpha;
};