
#include "AppFrame.h"
#include "ApplicationMain.h"
#include "ModeTitle.h"
#include "ModeGame.h"
#include"ModeColorOut.h"
#include"ModeColorIn.h"
#include "ModeSelect.h"

#include "UIServer.h"
#include "UIGameStart.h"
#include "UIQuit.h"
#include "UIDisplay.h"
#include "UIBlink.h"

bool ModeTitle::Initialize() {
	if (!base::Initialize()) { return false; }

	res::LoadGraph("res/UI/Confirm/ui_quitscreen_01.png");
	res::LoadGraph("res/UI/Confirm/ui_quityes_01.png");
	res::LoadGraph("res/UI/Confirm/ui_quitno_01.png");
	res::LoadGraph("res/UI/Confirm/ui_target_02.png");
	res::LoadGraph("res/UI/Confirm/confirm_disply.png");

	// server
	_ui = new UIServer();

	void* mdTitle = ModeServer::GetInstance()->Get("title");

	// UI用の画像ハンドル
	int cgStart, cgQuit, cgGet, cgBg, cgTitle;
	cgStart = res::LoadGraph("res/UI/Title/ui_gamestart_01.png");
	cgQuit = res::LoadGraph("res/UI/Title/ui_gamequit_01.png");
	cgGet = res::LoadGraph("res/UI/Title/ui_target_01.png");
	cgBg = res::LoadGraph("res/title_kari.png");
	cgTitle = res::LoadGraph("res/UI/Title/ui_title_01.png");

	/*           UIBase			param    Hundle x     y    w    h   layer	name */
	_ui->Add(new UIGameStart(), mdTitle, cgStart, 832, 768, 255, 63, 5, "start");
	_ui->Add(new UIQuit(), nullptr, cgQuit, 832, 832, 255, 63, 5, "quit");
	_ui->Add(new UIBlink(), nullptr, cgGet, 832, 768, 255, 63, 1, "get");
	_ui->Add(new UIDisplay(), nullptr, cgBg, 0, 0, 1920, 1080, 0, "bg");
	_ui->Add(new UIDisplay(), nullptr, cgTitle, 448, 384, 1072, 256, 0, "title");

	_ui->Search("start")->_selectNum = 0;
	_ui->Search("quit")->_selectNum = 1;

	_select = _ui->Search("start")->_selectNum;

	return true;
}

bool ModeTitle::Terminate() {
	base::Terminate();
	_ui->Terminate();
	delete _ui;
	return true;
}

bool ModeTitle::Process() {
	base::Process();
	int key = ApplicationMain::GetInstance()->GetKey();
	int trg = ApplicationMain::GetInstance()->GetPad()->GetTrgButton();

	//if (trg & INPUT_A && !ModeServer::GetInstance()->IsAdd("Out")) {
	//	auto func = [this]() {
	//		ModeServer::GetInstance()->Del(this);
	//		ModeServer::GetInstance()->Add(NEW ModeGame("1"), 10, "game");
	//	};

	//	ModeColorIn* colorIn = NEW ModeColorIn(30, true);
	//	ModeBase* mode = NEW ModeColorOut(colorIn,func);

	//	ModeServer::GetInstance()->Add(mode, 11, "Out");
	//}


	_ui->Process();

	// 上下でカーソルを移動する
	if (trg & INPUT_DPAD_UP) { _select--; }
	if (trg & INPUT_DPAD_DOWN) { _select++; }

	// 選択位置を上下ループ
	_select = (_select + MAX_OPTIONS) % MAX_OPTIONS;


	int close = 0;
	// 選択処理
	for (auto& ui : _ui->_vUI) {
		if (ui->_selectNum == _select) {
			std::string name = ui->_uiName;	// 選択している項目の名前
			_ui->SelectPosAdjustment(name, "get", 0, 0);
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

bool ModeTitle::Render() {
	base::Render();
	_ui->Render();

	return true;
}

