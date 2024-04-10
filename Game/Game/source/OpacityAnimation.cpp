#include"OpacityAnimation.h"
#include"SpriteText.h"

OpacityAnimation::OpacityAnimation(class SpriteText* text, int frame, float startAlpha, float endAlpha)
	:Animation(text, frame)
	, _startAlpha(startAlpha)
	, _endAlpha(endAlpha)
{

}

OpacityAnimation::OpacityAnimation(class SpriteText* text, int frame, float endAlpha)
	:Animation(text, frame)
	,_startAlpha(text->GetAlpha())
	,_endAlpha(endAlpha)
{

}

OpacityAnimation::~OpacityAnimation(){}

bool OpacityAnimation::Process() {

	Animation::Process();

	//アルファ値の算出
	float alpha = EasingLinear(_startAlpha, _endAlpha, _rate);
	_text->SetAlpha(alpha);

	return true;
}