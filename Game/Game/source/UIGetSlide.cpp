#include "UIGetSlide.h"

UIGetSlide::UIGetSlide(float initPosX, float initPosY, float endPosX, float endPosY, int frame)
	:UISlide(initPosX, initPosY, endPosX, endPosY, frame)
{
	_selecting = 0;
}

void UIGetSlide::SetSelecting(int select)
{
	_selecting = select;
}

int UIGetSlide::GetSelecting()
{
	return _selecting;
}
