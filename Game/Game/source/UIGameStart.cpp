
#include "UIGameStart.h"
#include "ModeTitle.h"
#include "ModeGame.h"
#include "ModeColorIn.h"
#include"ModeColorOut.h"

UIGameStart::UIGameStart() {
	
}

int UIGameStart::Selected() {
	ModeServer::GetInstance()->Del(ModeServer::GetInstance()->Get("ui"));
	// 次のモードを登録
	ModeColorIn* colorIn = new ModeColorIn(30, true);
	ModeBase* mode = new ModeColorOut(colorIn, ModeServer::GetInstance()->Get("title"), 30, new ModeGame("1"), 10, "game");
	ModeServer::GetInstance()->Add(mode, 11, "Out");
	return 1;
}