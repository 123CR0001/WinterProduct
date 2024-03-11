#pragma once
#include"Animation.h"

class TransformAnimation :public Animation {
public:
	TransformAnimation(class SpriteText* text,int frame, const Transform2& stratTransform,const Transform2& endTransform);
	TransformAnimation(class SpriteText* text,int frame, const Transform2& endTransform);

	~TransformAnimation();

	bool Process()override;

private:
	Transform2 _endTransform;

	Transform2 _startTransform;
};