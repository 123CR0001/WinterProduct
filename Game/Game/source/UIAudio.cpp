
#include "ModePause.h"
#include "UIAudio.h"
#include "UIDisplay.h"
#include "UIServer.h"

UIAudio::UIAudio(float initPosX, float initPosY, float endPosX, float endPosY, int frame)
	:UISlide(initPosX, initPosY, endPosX, endPosY, frame)
{
}

int UIAudio::Selected() {
	// UIServer‚ðŠi”[‚µ‚Ä‚¢‚é‚©
	if (typeid(UIServer*) != typeid(static_cast<UIServer*>(_param))) { return -1; }
	UIServer* server = static_cast<UIServer*>(_param);

	// AudioMeter‚ð’Ç‰Á
	server->Add(new UIDisplay(), nullptr, ResourceServer::LoadGraph("res/UI/Menu/ui_audiometer_01.png"),
		640, 64, 763, 831, 20, "audio_meter");

	return 0;
}
