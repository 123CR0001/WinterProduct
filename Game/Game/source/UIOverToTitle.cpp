
#include "UIOverToTitle.h"
#include "ModeTitle.h"

UIOverToTitle::UIOverToTitle() {

}

int UIOverToTitle::Selected() {
	// ゲームオーバーモードを削除
	ModeServer::GetInstance()->Del(ModeServer::GetInstance()->Get("GameOver"));
	// 次のモードを登録
	ModeServer::GetInstance()->Add(new ModeTitle(), 1, "title");
	return 0;
}
