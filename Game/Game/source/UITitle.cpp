#include "UITitle.h"
#include "ModeTitle.h"
#include "ModeColorIn.h"
#include"ModeColorOut.h"

UITitle::UITitle(float initPosX, float initPosY, float endPosX, float endPosY, int frame, std::string nowMode)
	:UISlide(initPosX, initPosY, endPosX, endPosY, frame) {
	_nowMode = nowMode;
}

int UITitle::Selected() {
	ModeServer::GetInstance()->Del(ModeServer::GetInstance()->Get(_nowMode.c_str()));
	ModeColorIn* colorIn = new ModeColorIn(30, true);
	ModeBase* mode = new ModeColorOut(colorIn, ModeServer::GetInstance()->Get(_nowMode.c_str()), 30, new ModeTitle(), 1, "title");
	ModeServer::GetInstance()->Add(mode, 11, "Out");
	return 0;
}
