
#include "UIGameStart.h"
#include "ModeTitle.h"
#include "ModeGame.h"
#include "ModeColorIn.h"
#include"ModeColorOut.h"

UIGameStart::UIGameStart() {
	
}

int UIGameStart::Selected() {
	ModeServer::GetInstance()->Del(ModeServer::GetInstance()->Get("ui"));

	auto func = []() {
		//タイトルの削除
		ModeBase* title = ModeServer::GetInstance()->Get("title");
		ModeServer::GetInstance()->Del(title);

		//ゲームの追加
		ModeServer::GetInstance()->Add(NEW ModeGame(), 10, "game");
	};

	// 次のモードを登録
	ModeColorIn* colorIn = new ModeColorIn(30, true);
	ModeBase* mode = new ModeColorOut(colorIn, func);
	ModeServer::GetInstance()->Add(mode, 11, "Out");

	return 1;
}