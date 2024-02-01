
#include "ApplicationMain.h"
#include "UISlide.h"

UISlide::UISlide(float initPosX, float initPosY, float endPosX, float endPosY, int frame) {
	_initPosX = initPosX;
	_initPosY = initPosY;
	_endPosX = endPosX;
	_endPosY = endPosY;
	_cnt = 0;
	_frame = frame;
}

void UISlide::Process() {
	_x = EasingLinear(_cnt, _initPosX, _endPosX, _frame);
	_y = EasingLinear(_cnt, _initPosY, _endPosY, _frame);
	if (_cnt < _frame) {
		_cnt++;
	}
}

int UISlide::Selected() {
	return 0;
}

