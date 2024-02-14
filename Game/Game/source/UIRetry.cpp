#include "UIRetry.h"
#include "ModeGame.h"
#include "ModeGameOver.h"

UIRetry::UIRetry() {

}

int UIRetry::Selected() {
	ModeServer::GetInstance()->Del(ModeServer::GetInstance()->Get("gameover"));
	// 次のモードを登録
	ModeServer::GetInstance()->Add(new ModeGame("1"), 1, "game");
	return 0;
}