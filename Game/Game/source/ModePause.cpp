
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
#include "UIToTitle.h"
#include "UIGetSlide.h"

bool ModePause::Initialize() {
	// 画像読みこみ
	int cgSystem;
	cgSystem = res::LoadGraph("res/UI/Menu/ui_system_01.png");
	res::LoadGraph("res/UI/Menu/ui_totitlescreen_01.png");
	res::LoadGraph("res/UI/Menu/ui_controlscreen_01.png");
	res::LoadGraph("res/UI/Menu/ui_audiometer_01.png");

	// UI用の画像ハンドル
	int cgLeftFrame, cgRightFrame, cgGet, cgControl, cgAudio, cgResume, cgToTitle;
	cgLeftFrame	= res::LoadGraph("res/UI/Menu/ui_menuframe_01.png");
	cgRightFrame	= res::LoadGraph("res/UI/Menu/ui_menuframe_02.png");
	cgGet				= res::LoadGraph("res/UI/Menu/ui_targetframe_01.png");
	cgControl			= res::LoadGraph("res/UI/Menu/ui_control_01.png");
	cgAudio			= res::LoadGraph("res/UI/Menu/ui_audio_01.png");
	cgResume		= res::LoadGraph("res/UI/Menu/ui_resume_01.png");
	cgToTitle			= res::LoadGraph("res/UI/Menu/ui_totitle_01.png");

	// ModeGameのポインタ
	void* mdGame = ModeServer::GetInstance()->Get("game");

	// server
	_ui = new UIServer();

	/*						UIBase												param  Hundle				x		  y			w			h			layer									name */
	_ui->Add(new UISlide(-506, 5, 0, 5, 10),		nullptr, cgSystem,		-506,  5,		379,		119,		BASIC_LAYER_VALUE, "system");
	_ui->Add(new UISlide(-576, 0, 0, 0, 10),		nullptr, cgLeftFrame,	-576,  0,		576,		1080,	LOW_LAYER_VALUE,	"left_frame");
	_ui->Add(new UISlide(1921, 0, 576, 0, 10),		nullptr, cgRightFrame,	1921,  0,		1344,	1080,	LOW_LAYER_VALUE,	"right_frame");
	_ui->Add(new UIContorl(-438, 192, 30, 192, 10),	this,	 cgControl,		-438,  192,	511,		99,		BASIC_LAYER_VALUE, "control");
	_ui->Add(new UIAudio(-438, 320, 30, 320, 10),	this,	 cgAudio,			-438,  320,	511,		99,		BASIC_LAYER_VALUE, "audio");
	_ui->Add(new UIResume(-438, 477, 30, 448, 10),	this,	 cgResume,		-438,  448,	511,		99,		BASIC_LAYER_VALUE, "resume");
	_ui->Add(new UIToTitle(-438, 925, 30, 925, 10),	this,	 cgToTitle,			-438,  925,	511,		99,		BASIC_LAYER_VALUE, "to_title");
	_ui->Add(new UIGetSlide(-438, 192, 30, 192, 10),nullptr, cgGet,				-438,  192,	9,			109,		HIGH_LAYER_VALUE,	"get");

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

	if (_bToGameFade) {
		_ui->Process();
		if (AllMoveFinished()) {
			_bToGameFade = false;
			_bToGeme = true;
		}
	}
	if (_bToGeme) {
		ModeServer::GetInstance()->Del(this);
	}
	else {

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
		// 選択決定後の処理
		SelectedItemProcess(result);
	}

	return true;
}

bool ModePause::Render() {
	DrawString(0, 100, "pause", GetColor(255, 0, 0));

	_ui->Render();

	return true;
}

void ModePause::SelectedItemProcess(int selectResult) {
	switch (selectResult) {
	case 0:	// 何も押されてない
		break;
	case 1: // 操作説明
		_ui->Add(new UIDisplay(), nullptr, res::LoadGraph("res/UI/Menu/ui_controlscreen_01.png"),
			640, 64, 763, 831, BASIC_LAYER_VALUE, "control_screen");
		break;
	case 2: // 音量調整
		_ui->Add(new UIDisplay(), nullptr, res::LoadGraph("res/UI/Menu/ui_audiometer_01.png"),
			640, 64, 763, 831, BASIC_LAYER_VALUE, "audio_meter");
		break;
	case 3: // ゲームに戻る
		PoseEndProcess();
		_bToGameFade = true;
		break;
	case 4:	// タイトルに戻る　その確認を行う
		ModeServer::GetInstance()->Add(new ModePauseConfirm(), 700, "pause_confirm");
		break;
	}
}

void ModePause::AdjustItemPos(std::string selectedItem) {
	float space = 32.0;
	if (_ui->Search(selectedItem) == _ui->Search("control")) {
		float contorlY = _ui->Search("control")->_y;
		float controlH = _ui->Search("control")->_h + 5;
		float audioY = contorlY + controlH + space;
		_ui->Search("audio")->_y = audioY;
		float audioH = _ui->Search("audio")->_h;
		float resumeY = audioY + audioH + space;
		_ui->Search("resume")->_y = resumeY;
	}
	else if (_ui->Search(selectedItem) == _ui->Search("audio")) {
		float audioY = _ui->Search("audio")->_y - 5;
		float controlH = _ui->Search("control")->_h;
		float controlY = audioY - space - controlH;
		_ui->Search("control")->_y = controlY;
		float audioH = _ui->Search("audio")->_h + 5;
		float resumeY = audioY + audioH + space;
		_ui->Search("resume")->_y = resumeY;
	}
	else if (_ui->Search(selectedItem) == _ui->Search("resume")) {
		float resumeY = _ui->Search("resume")->_y - 5;
		float audioH = _ui->Search("audio")->_h;
		float audioY = resumeY - space - audioH;
		_ui->Search("audio")->_y = audioY;
		float controlH = _ui->Search("control")->_h;
		float contorlY = audioY - space - controlH;
		_ui->Search("control")->_y = contorlY;
	}
}

void ModePause::PoseEndProcess() {
	for (auto& ui : _ui->_vUI) {
		if ((ui == _ui->Search("system"))
		 || (ui == _ui->Search("left_frame"))
		 || (ui == _ui->Search("right_frame"))
		 || (ui == _ui->Search("control"))
		 || (ui == _ui->Search("audio"))
		 || (ui == _ui->Search("resume"))
		 || (ui == _ui->Search("to_title"))
		 || (ui == _ui->Search("get"))) {
			float initX = ui->_endPosX;
			float initY = ui->_endPosY;
			float endX = ui->_initPosX;
			float endY = ui->_initPosY;
			int frame = ui->_frame;
			ui->SetSlideUIPosition(initX, initY, endX, endY, frame);
		}
	}
}

bool ModePause::AllMoveFinished() {
	for (auto& ui : _ui->_vUI) {
		if ((ui == _ui->Search("system"))
			|| (ui == _ui->Search("left_frame"))
			|| (ui == _ui->Search("right_frame"))
			|| (ui == _ui->Search("control"))
			|| (ui == _ui->Search("audio"))
			|| (ui == _ui->Search("resume"))
			|| (ui == _ui->Search("to_title"))
			|| (ui == _ui->Search("get"))) {
			if(ui->IsSlide()){ return false; }
		}
	}
	return true;
}



int ModePause::GetSelect() { return _select; }
