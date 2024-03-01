#include"ModeClear.h"
#include"ModeTitle.h"
#include"ModeGame.h"

#include "ApplicationMain.h"
#include"ApplicationGlobal.h"

#include "UIServer.h"
#include "UIDisplay.h"
#include "UIClearTime.h"
#include "UIClearTimeRank.h"
#include "UIBlink.h"
#include "UINextStage.h"

ModeClear::ModeClear(ModeGame* game) {
	_game = game;
	_stage = _game->GetStage();
	_uiServer = NEW UIServer();
}

bool ModeClear::Initialize() {

	_uiServer->Initialize();

	Load();

	_uiServer->Search("nextStage")->_selectNum = 0;

	return true;
}

bool ModeClear::Terminate() {
	return true;
}

bool ModeClear::Process() {
	base::Process();

	// このモードより下のレイヤーはProcess()を呼ばない
	ModeServer::GetInstance()->SkipProcessUnderLayer();

	auto trg = ApplicationMain::GetInstance()->GetPad()->GetTrgButton();

	_uiServer->Process();

	// 上下でカーソルを移動する
	if(trg & INPUT_DPAD_UP) { _select--; }
	if(trg & INPUT_DPAD_DOWN) { _select++; }

	// 選択位置を上下ループ
	_select = (_select + MAX_OPTIONS) % MAX_OPTIONS;


	// 選択処理
	for(const auto& ui : _uiServer->_vUI) {
		if(ui->_selectNum == -1) { continue; }
		if(ui->_selectNum == _select) {					// ui側に割り振られた番号と_selectの数字が同じか否か
			std::string name = ui->_uiName;					// 選択している項目の名前
			_uiServer->SelectPosAdjustment(name, "get", -64, -3);	// 選択中の項目にカーソルを重ねる	
			if(trg & INPUT_A) {
				ui->Selected();
			}
			break;
		}
	}

	if (trg & INPUT_A) { 
		// このモードを削除予約
		ModeServer::GetInstance()->Clear();
		// 次のモードを登録
		ModeServer::GetInstance()->Add(new ModeTitle(), 1, "title");
	}

	return true;
}

bool ModeClear::Render() {
	base::Render();
	SetFontSize(64);


	std::string str = "clear";
	int x = ApplicationMain::GetInstance()->DispSizeW()/2 - str.size() * 64 / 4;
	int y = ApplicationMain::GetInstance()->DispSizeH()/2 - 64 / 2;
	DrawFormatString(x, y, GetColor(255, 0, 0), "%s",str.c_str());
	SetFontSize(16);

	gGlobal._result.Render();

	_uiServer->Render();

	auto s = _stage.substr(2);
	DrawFormatString(0, 0, GetColor(255, 255, 255), "area:%s", s.c_str());

	return true;
}

void ModeClear::Load() {
	// スコア表示背景
	int cgScoreBg1, cgScoreBg2;
	cgScoreBg1 = res::LoadGraph("res/UI/Result/ui_scorebg_01.png");
	cgScoreBg2 = res::LoadGraph("res/UI/Result/ui_scorebg_02.png");
	_uiServer->Add(new UIDisplay(), nullptr, cgScoreBg1, 896, 64, 896, 360, LOW_LAYER_VALUE, "scoreBg1");
	_uiServer->Add(new UIDisplay(), nullptr, cgScoreBg2, 896, 780, 896, 232, LOW_LAYER_VALUE, "scoreBg2");

	_uiServer->Add(new UIClearTime(), nullptr, 0, 1490, 165, 40, 60, BASIC_LAYER_VALUE, "clearTime");

	//std::string timeRank = STR(gGlobal._result.GetTimeRank());
	//static_assert(3 == 3, "timeRank.size() != 3");
	//std::string strRank = timeRank.substr(1, 1);
	//int cgRank = res::LoadGraph("res/UI/Result/Emblem/ui_rankemblem_" + strRank.c_str() + ".png");
	//_uiServer->Add(new UIClearTimeRank(), nullptr, cgRank, 1024, 448, 640, 283, BASIC_LAYER_VALUE, "clearTimeRank");

	int cgNextStage, cgGet;
	cgNextStage = res::LoadGraph("res/UI/Result/ui_nextstage_01.png");
	cgGet = res::LoadGraph("res/UI/Result/ui_target_01.png");
	_uiServer->Add(new UINextStage(_stage), nullptr, cgNextStage, 1152, 796, 384, 64, BASIC_LAYER_VALUE, "nextStage");

	_uiServer->Add(new UIBlink(), nullptr, cgGet, 1152, 796, 512, 64, HIGH_LAYER_VALUE, "get");
}
