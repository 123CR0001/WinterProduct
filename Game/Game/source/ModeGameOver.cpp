#include"ModeGameOver.h"
#include "ApplicationMain.h"
#include"ApplicationGlobal.h"
#include"ModeTitle.h"

#include "UIServer.h"
#include "UIDisplay.h"
#include "UIRetry.h"
#include "UIOverToTitle.h"

ModeGameOver::ModeGameOver(ModeGame* game) {
	// ゲームモードのポインタを受け取る
	_game= game;
	// ゲームモードのステージ番号を受け取る
	_stage = _game->GetStage();
}

bool ModeGameOver::Initialize() {

	// 画像の読み込み
	int cgContinue, cgExit, cgFrame;
	res::LoadDivGraph("res/UI/GameOver/ui_gameoverbg_01.png", 6, 1, 6, 1920, 1080, _cgBg);
	cgContinue	= res::LoadGraph("res/UI/GameOver/ui_continue_01.png");
	cgExit		= res::LoadGraph("res/UI/GameOver/ui_exit_01.png");
	cgFrame		= res::LoadGraph("res/UI/GameOver/ui_gameoverframe_01.png");

	// UIServerの追加
	_uiServer = NEW UIServer();
	// UIServerの初期化
	_uiServer->Initialize();

	/*	UIの追加   UIBase				param    Hundle		 x		y	 w	  h	  layer				name */
	_uiServer->Add(NEW UIRetry(this),	nullptr, cgContinue, 448,	576, 448, 64, LOW_LAYER_VALUE,	"continue");
	_uiServer->Add(NEW UIOverToTitle(), nullptr, cgExit,	 1024,	576, 448, 64, LOW_LAYER_VALUE,	"exit");
	_uiServer->Add(NEW UIDisplay(),		nullptr, cgFrame,	 448,	576, 448, 64, BASIC_LAYER_VALUE,"get");

	// 選択肢として扱うものに、番号を割り振る
	_uiServer->Search("continue")->_selectNum = 0;
	_uiServer->Search("exit")->_selectNum = 1;

	// カーソルの初期位置を指定
	_select = _uiServer->Search("continue")->_selectNum;

	// アニメーション処理の変数の初期化
	_animCnt = 0;
	_frameCnt = 4;
	_motCnt = 0;

	//BGMを止める
	gGlobal._sndServer.StopType(SoundItemBase::TYPE::BGM);
	return true;
}

bool ModeGameOver::Terminate() {
	delete _uiServer;
	return true;
}

bool ModeGameOver::Process() {
	base::Process();

	// UIの処理
	_uiServer->Process();

	auto trg = ApplicationMain::GetInstance()->GetPad()->GetTrgButton();

	// 上下でカーソルを移動する
	if(trg & INPUT__DPAD_LEFT) { _select--; }
	if(trg & INPUT__DPAD_RIGHT) { _select++; }

	// 選択位置を上下ループ
	_select = (_select + MAX_OPTIONS) % MAX_OPTIONS;

	// 選択処理
	for(auto& ui : _uiServer->_vUI) {
		if(ui->_selectNum == -1) { continue; }					// 項目に指定されていないものはスキップ(-1のものは指定されていない)
		if(ui->_selectNum == _select) {							// ui側に割り振られた番号と_selectの数字が同じか否か
			std::string name = ui->_uiName;						// 選択している項目の名前
			_uiServer->SelectPosAdjustment(name, "get", 0, 0);	// 選択中の項目にカーソルを重ねる
			if(trg & INPUT_A) { ui->Selected(); }				// Aボタンで選択項目の処理を行う 
			break;
		}
	}

	// 背景のアニメーション処理
	_animCnt++;
	if(_frameCnt <= _animCnt) {
		if(_motCnt == 5) { _motCnt = 0; }
		_motCnt++;
		_animCnt = 0;
	}

	return true;
}

bool ModeGameOver::Render() {
	base::Render();

	// 画面サイズに合わせて画像を拡大縮小
	auto ConvertX = [](float posX) {
		float disp = ApplicationMain::GetInstance()->DispSizeW();
		float result = disp * posX / 1920;
		return result;
		};
	auto ConvertY = [](float posY) {
		float disp = ApplicationMain::GetInstance()->DispSizeH();
		float result = disp * posY / 1080;
		return result;
		};

	// 背景の描画
	float w = ConvertX(1920);
	float h = ConvertY(1080);
	DrawExtendGraph(0, 0, w, h, _cgBg[_motCnt], TRUE);

	// UIの描画
	_uiServer->Render();

	return true;
}