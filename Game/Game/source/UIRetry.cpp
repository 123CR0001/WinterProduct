#include "UIRetry.h"
#include "ModeGame.h"
#include "ModeGameOver.h"

UIRetry::UIRetry(ModeGameOver* over) {
	// gameover���[�h�̃|�C���^���󂯎��
	_over = over;
}

int UIRetry::Selected() {
	// gameover���[�h���폜
	ModeServer::GetInstance()->Del(ModeServer::GetInstance()->Get("GameOver"));
	// ���̃��[�h��o�^
	ModeServer::GetInstance()->Add(new ModeGame(_over->GetStage()), 1, "game");
	return 0;
}