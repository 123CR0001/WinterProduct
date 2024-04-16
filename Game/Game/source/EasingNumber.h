#pragma once
#include"Animation.h"
#include<functional>

//SpriteNumberの数字をイージング処理をする
//リザルトなどで使用する
class EasingNumber :public Animation {
public:
	EasingNumber(class SpriteNumber* sprite, int frame, std::function<float(float, float, float, float)>func = EasingInQuad);
	~EasingNumber();

	bool Process()override;

private:

	std::function<float(float ,float ,float ,float)>_func;				//使用するイージングの関数

	class SpriteNumber* _spriteNumber;									//イージングするSpriteNumber	

	int _number;														//イージングする数字	
};