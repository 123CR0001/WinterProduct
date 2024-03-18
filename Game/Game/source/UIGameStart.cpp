
#include "UIGameStart.h"
#include "ModeTitle.h"
#include "ModeGame.h"
#include "ModeColorIn.h"
#include"ModeColorOut.h"
#include"ModeSelect.h"
#include"ModeStageSelect.h"

UIGameStart::UIGameStart() {
	
}

int UIGameStart::Selected() {

	auto func = [this]() {
		//�^�C�g���̍폜

		ModeServer::GetInstance()->Del(ModeServer::GetInstance()->Get("ui"));
		ModeBase* title = ModeServer::GetInstance()->Get("title");
		ModeServer::GetInstance()->Del(title);

		//�Q�[���̒ǉ�
		ModeServer::GetInstance()->Add(NEW ModeSelect(), 10, "select");
	};

	// ���̃��[�h��o�^
	ModeColorIn* colorIn = new ModeColorIn(30, true);
	ModeBase* mode = new ModeColorOut(colorIn, func);
	ModeServer::GetInstance()->Add(mode, 11, "Out");

	return 1;
}