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

	//���ۂ̕`�悷��ʒu�ɉ��Z�⌸�Z�����Ȃ��̂́A�`��ʒu���ǂ�ǂ񂸂�Ă����Ȃ��悤�ɂ��邽��
	Vector2 pos = GetPos();

	do {

		//�`�悷�鐔��
		SetHandle(_handles[number % 10]);

		Vector2 diff = pos - GetPos();

		//�`�悷��ʒu��ύX
		SpriteText::Draw(MGetTranslate(VGet(diff.x, diff.y, 0.f)));

		//�`�悷��ʒu�����ɂ��炷
		pos.x -= GetSize().x;

		//�`�悵�����͂�������Ȃ��̂ŁA�ꌅ�J�艺����
		number /= 10;

		//�`�悷�錅�����w�肳��Ă�����A����ȍ~�`�悵�Ȃ�
		if (digit == 0)break;

		digit--;
	} while (number || digit > 0);
	//_drawDigit�����̐��Ȃ�A�`�悷�錅���́A_number�̌��Ɉˑ�����
}