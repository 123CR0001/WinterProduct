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
	float num = _func(_frameCnt, 0, _number, _frame);

	_spriteNumber->SetNumber(static_cast<int>(num));

	return true;
}