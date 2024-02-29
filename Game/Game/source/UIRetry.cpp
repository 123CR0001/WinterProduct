#include "UIRetry.h"
#include "ModeGame.h"
#include "ModeGameOver.h"

UIRetry::UIRetry(ModeGameOver* over) {
	// gameoverモードのポインタを受け取る
	_over = over;
}

int UIRetry::Selected() {
	// gameoverモードを削除
	ModeServer::GetInstance()->Del(ModeServer::GetInstance()->Get("GameOver"));
	// 次のモードを登録
	ModeServer::GetInstance()->Add(new ModeGame(_over->GetStage()), 1, "game");
	return 0;
}