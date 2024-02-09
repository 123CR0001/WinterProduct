
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
		//�^�C�g���̍폜
		ModeBase* title = ModeServer::GetInstance()->Get("title");
		ModeServer::GetInstance()->Del(title);

		//�Q�[���̒ǉ�
		ModeServer::GetInstance()->Add(NEW ModeGame(), 10, "game");
	};

	// ���̃��[�h��o�^
	ModeColorIn* colorIn = new ModeColorIn(30, true);
	ModeBase* mode = new ModeColorOut(colorIn, func);
	ModeServer::GetInstance()->Add(mode, 11, "Out");

	return 1;
}