#include"SpriteNumber.h"

SpriteNumber::SpriteNumber(int number,int digit)
	:SpriteText()
	,_number(number)
	,_drawDigit(digit)
{

}

SpriteNumber::~SpriteNumber()
{

}

void SpriteNumber::Draw(MATRIX mView) {

	int number = _number;

	int digit = _drawDigit;

	//実際の描画する位置に加算や減算をしないのは、描画位置がどんどんずれていかないようにするため
	Vector2 pos = GetPos();

	do {

		//描画する数字
		SetHandle(_handles[number % 10]);

		Vector2 diff = pos - GetPos();

		//描画する位置を変更
		SpriteText::Draw(MGetTranslate(VGet(diff.x, diff.y, 0.f)));

		//描画する位置を左にずらす
		pos.x -= GetSize().x;

		//描画した桁はもういらないので、一桁繰り下げる
		number /= 10;

		//描画する桁数が指定されていたら、それ以降描画しない
		if (digit == 0)break;

		digit--;
	} while (number || digit > 0);
	//_drawDigitが負の数なら、描画する桁数は、_numberの桁に依存する
}