
#include "ApplicationMain.h"
#include "ModePause.h"
#include "ModePauseConfirm.h"
#include "ModeGame.h"
#include "UIServer.h"
#include "UIDisplay.h"
#include "UISlide.h"
#include "UIContorl.h"
#include "UIAudio.h"
#include "UIResume.h"
#include "UIToConfirm.h"
#include "UIGetSlide.h"

bool ModePause::Initialize() {
	// 画像読みこみ
	int cgSystem;
	cgSystem = res::LoadGraph("res/UI/Menu/ui_system_01.png");

	// UI用の画像ハンドル
	int cgLeftFrame, cgRightFrame, cgGet, cgControl, cgAudio, cgResume, cgToTitle;
	cgLeftFrame	 = res::LoadGraph("res/UI/Menu/ui_menuframe_01.png");
	cgRightFrame = res::LoadGraph("res/UI/Menu/ui_menuframe_02.png");
	cgGet		 = res::LoadGraph("res/UI/Menu/ui_targetframe_01.png");
	cgControl	 = res::LoadGraph("res/UI/Menu/ui_control_01.png");
	cgAudio		 = res::LoadGraph("res/UI/Menu/ui_audio_01.png");
	cgResume	 = res::LoadGraph("res/UI/Menu/ui_resume_01.png");
	cgToTitle	 = res::LoadGraph("res/UI/Menu/ui_totitle_01.png");

	// ModeGameのポインタ
	void* mdGame = ModeServer::GetInstance()->Get("game");

	// server
	_ui = new UIServer();

	/*			 UIBase								  param    Hundle		x	   y	w	  h		layer			   name */
	_ui->Add(new UISlide(-506, 5, 0, 5, 10),		  nullptr, cgSystem,		-506,  5,	379,  119,	BASIC_LAYER_VALUE, "system");
	_ui->Add(new UISlide(-576, 0, 0, 0, 10),		  nullptr, cgLeftFrame,	-576,  0,	576,  1080,	LOW_LAYER_VALUE,   "left_frame");
	_ui->Add(new UISlide(1921, 0, 576, 0, 10),		  nullptr, cgRightFrame,	1921,  0,	1344, 1080,	LOW_LAYER_VALUE,   "right_frame");
	_ui->Add(new UIContorl(-438, 192, 30, 192, 10),	  _ui,	  cgControl,	-438,  192,	511,  99,	BASIC_LAYER_VALUE, "control");
	_ui->Add(new UIAudio(-438, 320, 30, 320, 10),	  _ui,	  cgAudio,		-438,  320,	511,  99,	BASIC_LAYER_VALUE, "audio");
	_ui->Add(new UIResume(-438, 477, 30, 448, 10),	  _ui,	  cgResume,		-438,  448,	511,  99,	BASIC_LAYER_VALUE, "resume");
	_ui->Add(new UIToConfirm(-438, 925, 30, 925, 10), _ui,	  cgToTitle,	-438,  925,	511,  99,	BASIC_LAYER_VALUE, "to_title");
	_ui->Add(new UIGetSlide(-438, 192, 30, 192, 10),  nullptr, cgGet,		-438,  192,	9,	  109,	HIGH_LAYER_VALUE,  "get");

	// 選択肢として扱うものに、番号を割り振る
	_ui->Search("control")	-> _selectNum = 0;
	_ui->Search("audio")	-> _selectNum = 1;
	_ui->Search("resume")	-> _selectNum = 2;
	_ui->Search("to_title")	-> _selectNum = 3;

	// カーソルの初期位置を指定
	_select = _ui->Search("control")->_selectNum;

	// タイトルに戻る用のフラグ falseにしておく
	_bToGeme = false;
	_bToGameFade = false;
	_bClosePause = false;

	return true;
}

bool ModePause::Terminate() {
	_ui->Terminate();
	delete _ui;
	return true;
}

bool ModePause::Process() {
	// このモードよりレイヤーが低いものは処理を止める
	ModeServer::GetInstance()->SkipProcessUnderLayer();

	_ui->Process();

	auto trg = ApplicationMain::GetInstance()->GetPad()->GetTrgButton();

	_ui->Process();

	bool bProcess = true;
	if (_ui->IsRegistering("control_screen")) {
		bProcess = false;
		if (trg & INPUT_B) { _ui->Del("control_screen"); }
	}
	if (_ui->IsRegistering("audio_meter")) {
		bProcess = false;
		if (trg & INPUT_B) { _ui->Del("audio_meter"); }
	}

	if (bProcess) {
		// 上下でカーソルを移動する
		if (trg & INPUT_DPAD_UP) { _select--; }
		if (trg & INPUT_DPAD_DOWN) { _select++; }
	}

	// 選択位置を上下ループ
	_select = (_select + MAX_OPTIONS) % MAX_OPTIONS;

	int result = 0;
	// 選択処理
	for (auto& ui : _ui->_vUI) {
		if (ui == _ui->Search("get")) { continue; }			// 
		if (ui->_selectNum == _select) {					// ui側に割り振られた番号と_selectの数字が同じか否か
			std::string name = ui->_uiName;					// 選択している項目の名前
			AdjustItemPos(name);
			_ui->SelectPosAdjustment(name, "get", 0, -5);	// 選択中の項目にカーソルを重ねる
			if (trg & INPUT_A) { result = ui->Selected(); }	// Aボタンで選択項目の処理を行う 
			break;
		}
	}

	if (result == 1) {
		_bClosePause= true;
	}
	if (_bClosePause && AllMoveFinished()) {
		ModeServer::GetInstance()->Del(this);
	}

	return true;
}

bool ModePause::Render() {
	DrawString(0, 100, "pause", GetColor(255, 0, 0));

	float oldW = 0, oldY = 0, oldH = 0;
	std::string selectName;
	for (const auto& ui : _ui->_vUI) {
		if (ui == _ui->Search("get")) { continue; }
		if (ui->_selectNum == _select) {
			selectName = ui->_uiName;
			oldW = ui->_w;
			oldY = ui->_y;
			oldH = ui->_h;
			ui->_w += 51;
			ui->_y -= 5;
			ui->_h += 10;
			break;
		}
	}

	_ui->Render();

	// ループ外で選択されたUI要素を取得
	auto selectUI = _ui->Search(selectName);

	// 元の値に戻す
	selectUI->_w = oldW;
	selectUI->_y = oldY;
	selectUI->_h = oldH;

	return true;
}

void ModePause::AdjustItemPos(std::string selectedItem) {
	float stage1Y = _ui->Search("control")->_y;
	float stage2Y = _ui->Search("audio")->_y;
	float stage3Y = _ui->Search("resume")->_y;

	if (_ui->Search(selectedItem) == _ui->Search("control")) {
		stage2Y += 5;
		stage3Y += 5;
	}
	else if (_ui->Search(selectedItem) == _ui->Search("audio")) {
		stage1Y -= 5;
		stage3Y += 5;
	}
	else if (_ui->Search(selectedItem) == _ui->Search("resume")) {
		stage1Y -= 5;
		stage2Y -= 5;
	}

	_ui->Search("control")->_y = stage1Y;
	_ui->Search("audio")->_y = stage2Y;
	_ui->Search("resume")->_y = stage3Y;
}


bool ModePause::AllMoveFinished() {
	// サーバーからUIを検索して、スライドさせる
	std::vector<std::string> targetNames = { "system", "left_frame", "right_frame", "control", "audio", "resume", "to_title", "get" };
	for (auto& name : targetNames) {
		auto ui = _ui->Search(name);
		if (ui) {
			if (ui->IsSlide()) { return false; }
		}
	}
	return true;
}
