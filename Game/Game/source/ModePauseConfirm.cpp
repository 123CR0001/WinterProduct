
#include "ApplicationMain.h"
#include "ModePauseConfirm.h"
#include "UIServer.h"
#include "UIDisplay.h"
#include "UIBackToTitle.h"
#include "UINo.h"
#include "UIBlink.h"

bool ModePauseConfirm::Initialize()
{
	// UI用の画像ハンドル
	int cgYes, cgNo, cgGet, cgBg, cgQuit;
	cgYes	= res::LoadGraph("res/UI/Confirm/ui_quityes_01.png");
	cgNo	= res::LoadGraph("res/UI/Confirm/ui_quitno_01.png");
	cgGet	= res::LoadGraph("res/UI/Confirm/ui_target_02.png");
	cgBg	= res::LoadGraph("res/UI/Confirm/confirm_disply.png");
	cgQuit	= res::LoadGraph("res/UI/Menu/ui_totitlescreen_01.png");

	// ModeTitle/ModePauseのポインタ
	void* mdPause = ModeServer::GetInstance()->Get("pause");

	// server
	_ui = new UIServer();

	/*			 UIBase				param	 Hundle	 x	   y	w	  h		layer			   name */
	_ui->Add(new UIBackToTitle(),	this, cgYes,	 640,  768, 255,  63,	HIGH_LAYER_VALUE,  "yes");
	_ui->Add(new UINo(),			nullptr, cgNo,	 1024, 768, 255,  63,	HIGH_LAYER_VALUE,  "no");
	_ui->Add(new UIBlink(),			nullptr, cgGet,	 640,  768, 255,  63,	BASIC_LAYER_VALUE, "get_confirm");
	_ui->Add(new UIDisplay(),		nullptr, cgBg,	 0,	   0,	1920, 1080,	LOW_LAYER_VALUE,   "bg_confirm");
	_ui->Add(new UIDisplay(),		nullptr, cgQuit, 0,	   256, 1920, 640,	LOW_LAYER_VALUE,   "quit_confirm");

	// 選択肢として扱うものに、番号を割り振る
	_ui->Search("yes")->_selectNum = 0;
	_ui->Search("no")->_selectNum = 1;

	// カーソルの初期位置を指定
	_select = _ui->Search("no")->_selectNum;

	_bToTitle = false;

	return true;
}

bool ModePauseConfirm::Terminate()
{
	_ui->Terminate();
	delete _ui;
	return true;
}

bool ModePauseConfirm::Process()
{
	ModeServer::GetInstance()->SkipProcessUnderLayer();

	auto trg = ApplicationMain::GetInstance()->GetPad()->GetTrgButton();

	_ui->Process();

	// 上下でカーソルを移動する
	if (trg & INPUT__DPAD_LEFT) { _select--; }
	if (trg & INPUT__DPAD_RIGHT) { _select++; }

	// カーソル位置を上下ループ
	_select = (_select + MAX_OPTIONS) % MAX_OPTIONS;

	// 選択肢の位置を変更する
	int close = 0;
	// 選択処理
	for (auto& ui : _ui->_vUI) {
		if (ui == _ui->Search("get_confirm") || ui->_layer < BASIC_LAYER_VALUE) { continue; }
		if (ui->_selectNum == _select) {
			std::string name = ui->_uiName;	// 選択している項目の名前
			_ui->SelectPosAdjustment(name, "get_confirm", 0, 0);
			if (trg & INPUT_A) {	// 決定で選択項目のSelected()を呼ぶ
				close = _ui->Search(name)->Selected();
			}
			break;
		}
	}

	if (close != 0) {
		ModeServer::GetInstance()->Del(this);
	}

	return true;
}

bool ModePauseConfirm::Render()
{
	DrawString(0, 100, "confirm", GetColor(255, 0, 0));
	_ui->Render();
	return true;
}
