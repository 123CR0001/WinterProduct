
#include "UIGameStart.h"
#include "ModeTitle.h"
#include "ModeGame.h"
#include "ModeColorIn.h"
#include"ModeColorOut.h"

UIGameStart::UIGameStart() {
	
}

int UIGameStart::Selected() {

	auto func = [this]() {
		//ƒ^ƒCƒgƒ‹‚Ìíœ

		ModeServer::GetInstance()->Del(ModeServer::GetInstance()->Get("ui"));
		ModeBase* title = ModeServer::GetInstance()->Get("title");
		ModeServer::GetInstance()->Del(title);

		//ƒQ[ƒ€‚Ì’Ç‰Á
		ModeServer::GetInstance()->Add(NEW ModeGame("1"), 10, "game");
	};

	// Ÿ‚Ìƒ‚[ƒh‚ğ“o˜^
	ModeColorIn* colorIn = new ModeColorIn(30, true);
	ModeBase* mode = new ModeColorOut(colorIn, func);
	ModeServer::GetInstance()->Add(mode, 11, "Out");

	return 1;
}