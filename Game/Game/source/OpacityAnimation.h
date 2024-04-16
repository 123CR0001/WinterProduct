#pragma once
#include"Animation.h"

class OpacityAnimation :public Animation{
public:
	OpacityAnimation(class SpriteText* text, int frame,float startAlpha,float endAlpha);
	OpacityAnimation(class SpriteText* text, int frame, float endAlpha);
	~OpacityAnimation();

	bool Process()override;
private:
	const float _startAlpha;	//最初のアルファ値

	const float _endAlpha;		//最終のアルファ値
	
};