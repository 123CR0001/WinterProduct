
#include "UIBackToTitle.h"
#include "ModePause.h"
#include "ModeTitle.h"

UIBackToTitle::UIBackToTitle() {

}

int UIBackToTitle::Selected() {
	ModeServer::GetInstance()->Del(ModeServer::GetInstance()->Get("Pause"));
	ModeServer::GetInstance()->Del(ModeServer::GetInstance()->Get("game"));
	ModeServer::GetInstance()->Del(ModeServer::GetInstance()->Get("effectsample"));
	ModeServer::GetInstance()->Del(ModeServer::GetInstance()->Get("Debug"));
	ModeServer::GetInstance()->Add(new ModeTitle(), 1, "title");
	return 1;
}
