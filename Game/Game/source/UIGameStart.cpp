
#include "UIGameStart.h"
#include "ModeTitle.h"
#include "ModeGame.h"
#include "ModeColorIn.h"
#include"ModeColorOut.h"
#include"ModeStageSelect.h"

UIGameStart::UIGameStart() {
	
}

int UIGameStart::Selected() {

	auto func = [this]() {
		//タイトルの削除

		ModeServer::GetInstance()->Del(ModeServer::GetInstance()->Get("ui"));
		ModeBase* title = ModeServer::GetInstance()->Get("title");
		ModeServer::GetInstance()->Del(title);

		//ゲームの追加
		ModeServer::GetInstance()->Add(NEW ModeStageSelect(), 10, "select");
	};

	// 次のモードを登録
	ModeColorIn* colorIn = new ModeColorIn(30, true);
	ModeBase* mode = new ModeColorOut(colorIn, func);
	ModeServer::GetInstance()->Add(mode, 11, "Out");

	return 1;
}