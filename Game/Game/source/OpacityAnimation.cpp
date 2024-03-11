#include"OpacityAnimation.h"
#include"SpriteText.h"

OpacityAnimation::OpacityAnimation(class SpriteText* text, int frame, float startAlpha, float endAlpha)
	:Animation(text, frame)
	, _startAlpha(startAlpha)
	, _endalpha(endAlpha)
{

}

OpacityAnimation::OpacityAnimation(class SpriteText* text, int frame, float endAlpha)
	:Animation(text, frame)
	,_startAlpha(text->GetAlpha())
	,_endalpha(endAlpha)
{

}

OpacityAnimation::~OpacityAnimation(){}

bool OpacityAnimation::Process() {

	Animation::Process();

	float alpha = EasingLinear(_startAlpha, _endalpha, _rate);
	_text->SetAlpha(alpha);

	return true;
}