
#include "UINextStage.h"

#include "ModeGame.h"
#include "ModeTitle.h"
#include "ModeColorIn.h"
#include"ModeColorOut.h"

UINextStage::UINextStage(std::string stage) {
	_stage = stage;
}

int UINextStage::Selected() {
	std::string stage = _stage.substr(0, 1);
	std::string area = _stage.substr(2);
	int stageNum = std::stoi(stage);
	int areaNum = std::stoi(area);
	if(areaNum == 4) {
		if(stageNum == 3) {
			auto func = [this]() {
				// �Q�[���N���A�̍폜
				ModeServer::GetInstance()->Del(ModeServer::GetInstance()->Get("GameClear"));
				// �^�C�g���̒ǉ�
				ModeServer::GetInstance()->Add(NEW ModeTitle(), 1, "title");
				};
			// ���̃��[�h��o�^
			ModeColorIn* colorIn = new ModeColorIn(30, true);
			ModeBase* mode = new ModeColorOut(colorIn, func);
			ModeServer::GetInstance()->Add(mode, 11, "Out");
			return 0;
		}
		else {
			stageNum++;
			areaNum = 1;
		}
	}
	else {
		areaNum++;
	}

	std::string nextStage = std::to_string(stageNum) + "_" + std::to_string(areaNum);
	_stage = nextStage;

	auto func = [this]() {
		//�^�C�g���̍폜
		ModeServer::GetInstance()->Del(ModeServer::GetInstance()->Get("GameClear"));

		//�Q�[���̒ǉ�
		ModeServer::GetInstance()->Add(NEW ModeGame(_stage), 1, "game");
		};

	// ���̃��[�h��o�^
	ModeColorIn* colorIn = new ModeColorIn(30, true);
	ModeBase* mode = new ModeColorOut(colorIn, func);
	ModeServer::GetInstance()->Add(mode, 11, "Out");

	return 0;
}
