#include "UIRetry.h"
#include "ModeGame.h"
#include "ModeGameOver.h"

UIRetry::UIRetry() {

}

int UIRetry::Selected() {
	ModeServer::GetInstance()->Del(ModeServer::GetInstance()->Get("gameover"));
	// ŽŸ‚Ìƒ‚[ƒh‚ð“o˜^
	ModeServer::GetInstance()->Add(new ModeGame(), 1, "game");
	return 0;
}