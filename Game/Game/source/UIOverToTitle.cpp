
#include "UIOverToTitle.h"
#include "ModeTitle.h"

UIOverToTitle::UIOverToTitle() {

}

int UIOverToTitle::Selected() {
	// �Q�[���I�[�o�[���[�h���폜
	ModeServer::GetInstance()->Del(ModeServer::GetInstance()->Get("GameOver"));
	// ���̃��[�h��o�^
	ModeServer::GetInstance()->Add(new ModeTitle(), 1, "title");
	return 0;
}
