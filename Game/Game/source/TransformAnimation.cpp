#include"TransformAnimation.h"
#include"SpriteText.h"

TransformAnimation::TransformAnimation(SpriteText* text,int frame, const Transform2& startTransform, const Transform2& endTransform)
	:Animation(text, frame)
	,_startTransform(startTransform)
	,_endTransform(endTransform)

{

}

TransformAnimation::TransformAnimation(SpriteText* text,int frame, const Transform2& endTransform) 
	:Animation(text,frame)
	, _startTransform(text->GetTransform())
	, _endTransform(endTransform)
{

}

TransformAnimation::~TransformAnimation(){}

bool TransformAnimation::Process() {

	if (!Animation::Process()) { return true; };

	//‰ñ“]
	float rad = EasingLinear(_startTransform.rad, _endTransform.rad, _rate);

	//Šg‘å
	float zoom = EasingLinear(_startTransform.zoom, _endTransform.zoom, _rate);

	//ˆÚ“®
	Vector2 pos = Vector2::Lerp(_startTransform.pos, _endTransform.pos, _rate);

	_text->SetTransform(Transform2(rad, zoom, pos));


	return true;
}
