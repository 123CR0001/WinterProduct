
#include "UIGameEnd.h"
#include "ModeTitle.h"
#include"ApplicationMain.h"

UIGameEnd::UIGameEnd() {
}

int UIGameEnd::Selected() {
	ApplicationMain::GetInstance()->GameEnd();
	return 1;
}