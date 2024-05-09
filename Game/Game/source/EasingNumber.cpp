#include"EasingNumber.h"
#include"SpriteNumber.h"

EasingNumber::EasingNumber(SpriteNumber* sprite, int frame, std::function<float(float, float, float, float)>func)
	:Animation(sprite, frame)
	, _func(func)
	,_spriteNumber(sprite)
	,_number(sprite->GetNumber())
{

}

EasingNumber::~EasingNumber(){}

bool EasingNumber::Process() {

	Animation::Process();

	//イージングで描画する数値を決める
	float num = _func(static_cast<float>(_frameCnt), 0.f, static_cast<float>(_number), static_cast<float>(_frame));

	_spriteNumber->SetNumber(static_cast<int>(num));

	return true;
}