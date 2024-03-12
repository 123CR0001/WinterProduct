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

	int number = floor(_number);

	if (number < 0)return;

	int digit = _drawDigit;

	//ÀÛ‚Ì•`‰æ‚·‚éˆÊ’u‚É‰ÁZ‚âŒ¸Z‚ğ‚µ‚È‚¢‚Ì‚ÍA•`‰æˆÊ’u‚ª‚Ç‚ñ‚Ç‚ñ‚¸‚ê‚Ä‚¢‚©‚È‚¢‚æ‚¤‚É‚·‚é‚½‚ß
	Vector2 pos = GetPos();

	do {

		//•`‰æ‚·‚é”š
		SetHandle(_handles[number % 10]);

		Vector2 diff = pos - GetPos();

		//•`‰æ‚·‚éˆÊ’u‚ğ•ÏX
		SpriteText::Draw(MGetTranslate(VGet(diff.x, diff.y, 0.f)));

		//•`‰æ‚·‚éˆÊ’u‚ğ¶‚É‚¸‚ç‚·
		pos.x -= GetSize().x;

		//•`‰æ‚µ‚½Œ…‚Í‚à‚¤‚¢‚ç‚È‚¢‚Ì‚ÅAˆêŒ…ŒJ‚è‰º‚°‚é
		number /= 10;

		//•`‰æ‚·‚éŒ…”‚ªw’è‚³‚ê‚Ä‚¢‚½‚çA‚»‚êˆÈ~•`‰æ‚µ‚È‚¢
		if (digit == 0)break;

		digit--;
	} while (number || digit > 0);
	//_drawDigit‚ª•‰‚Ì”‚È‚çA•`‰æ‚·‚éŒ…”‚ÍA_number‚ÌŒ…‚ÉˆË‘¶‚·‚é
}