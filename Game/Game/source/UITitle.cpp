#include "UITitle.h"
#include "ModeTitle.h"
#include "ModeColorIn.h"
#include"ModeColorOut.h"

UITitle::UITitle(float initPosX, float initPosY, float endPosX, float endPosY, int frame, std::string nowMode)
	:UISlide(initPosX, initPosY, endPosX, endPosY, frame) {
	_nowMode = nowMode;
}

int UITitle::Selected() {

	ModeColorIn* colorIn = new ModeColorIn(30, true);

	auto func = [this]() {
		ModeServer::GetInstance()->Del(ModeServer::GetInstance()->Get(_nowMode.c_str()));
		ModeServer::GetInstance()->Add(new ModeTitle(), 1, "title");
	};
	ModeBase* mode = new ModeColorOut(colorIn, func, 30);
	ModeServer::GetInstance()->Add(mode, 11, "Out");
	return 0;
}
