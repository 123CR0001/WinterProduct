
#include "UIServer.h"
#include "UIContorl.h"
#include "UIDisplay.h"

UIContorl::UIContorl(float initPosX, float initPosY, float endPosX, float endPosY, int frame)
	:UISlide(initPosX, initPosY, endPosX, endPosY, frame)
{
 }

int UIContorl::Selected() {
	// UIServerを格納しているか
	if (typeid(UIServer*) != typeid(static_cast<UIServer*>(_param))) { return -1; }
	UIServer* server = static_cast<UIServer*>(_param);
	// ControlScreenを追加
	server->Add(new UIDisplay(), nullptr, ResourceServer::LoadGraph("res/UI/Menu/ui_controlscreen_01.png"),
		640, 64, 763, 831, 20, "control_screen");

	return 0;
}

