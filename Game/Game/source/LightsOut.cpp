#include"ModeGame.h"
#include"UIHrsMinSec.h"
#include"SpriteTextFlipAnimation.h"
#include"UI.h"
#include"MyUIServer.h"
#include"LightsOut.h"

#include"ModeColorIn.h"
#include"ModeColorOut.h"

#include"TimeLine.h"

#include"MyUIServer.h"
#include"UISpriteText.h"

#include"ObjectServer.h"
#include"Player.h"

#include"ApplicationMain.h"
#include"ApplicationGlobal.h"

LightsOut::LightsOut(ModeGame* game) 
	:_game(game)
	,_timerBg(NEW SpriteTextFlipAnimation(8,false))
	,_timer(NEW UIHrsMinSec(1,0,Transform2(Vector2(500.f,100.f)),Vector2(80.f,100.f)))
	,_noise(NEW SpriteTextFlipAnimation(8, true))
	,_hud(NEW SpriteText())
	,_frameCnt(300)
	,_useTimes(1)
	,_state(STATE::kNone)
{

	int screenWidth = ApplicationMain::GetInstance()->DispSizeW();
	int screenHeight = ApplicationMain::GetInstance()->DispSizeH();

	_timerBg->Stop();
	_timerBg->LoadDivText("res/UI/Game/Timer/ui_timerbg_01.png", 5, 1, 5, 600, 150);
	_timerBg->SetPos(Vector2(200.f, 100.f));
	_timerBg->SetSize(Vector2(600.f,150.f));

	_noise->LoadDivText("res/Effect/ui_nightscope_1.png", 6, 1, 6, 1920, 1080);
	_noise->SetSize(Vector2(screenWidth, screenHeight));
	_noise->SetPos(Vector2((float)screenWidth / 2,(float)screenHeight / 2));
	_noise->SetAlpha(0.f);

	_hud->SetHandle(LoadGraph("res/hud_01.png"));
	_hud->SetSize(Vector2(screenWidth, screenHeight));
	_hud->SetPos(Vector2((float)screenWidth / 2, (float)screenHeight / 2));
	_hud->SetAlpha(0.f);

	_timer->SetIsDraw(false);

	//UIを追加
	_game->GetUIServer()->AddUI(NEW UISpriteText(_noise));
	_game->GetUIServer()->AddUI(NEW UISpriteText(_hud));
	_game->GetUIServer()->AddUI(NEW UISpriteText(_timerBg));
	_game->GetUIServer()->AddUI(_timer);
}

LightsOut::~LightsOut(){

}

bool LightsOut::Process() {

	switch (_state) {
	case STATE::kNone:
		break;
	case STATE::kStart: {

		//タイマーの背景のアニメーションを始める
		_timerBg->Play();

		//残りの使用回数を減らす
		_useTimes--;

		//タイマーを30フレーム後に描画されるようにする
		auto func = [this]() {
			_timer->SetIsDraw(true);
		};
		_game->GetTimeLine()->AddLine(30, func);

		_game->GetObjectServer()->GetPlayer()->SetIsLightsOut(true);

		//フェードインアウトを利用した演出
		ModeServer::GetInstance()->Add(NEW ModeColorOut(NEW ModeColorIn(10, true), nullptr, 10), 100, "Out");

		//
		_noise->SetAlpha(1.f);
		_hud->SetAlpha(1.f);

		_state = STATE::kProcess;
		break;
	}

	case STATE::kProcess: {
		_frameCnt--;

		if (_frameCnt < 0) {
			
			_state = STATE::kEnd;
		}

		_timer->SetSecondTime(_frameCnt / 60);

		if (_frameCnt % 60 == 0) {
			//SE
		}

		break;
	}

	case STATE::kEnd: {
		
		//アニメーションを逆再生させる
		_timerBg->Reverse();

		{
			//30フレーム後に描画されないようにする
			auto func = [this]() {
				_timer->SetIsDraw(false);
			};
			_game->GetTimeLine()->AddLine(30, func);
		}

		//フェードインアウトを利用した演出
		ModeServer::GetInstance()->Add(NEW ModeColorOut(NEW ModeColorIn(10, true), nullptr, 10), 100, "Out");

		if (_useTimes == 0) {
			//120フレーム後にゲームオーバーかゲームクリアを決める
			auto func = [this]() {
				_game->SwitchOverOrClear();
			};

			_game->GetTimeLine()->AddLine(120, func);
		}
		_game->GetObjectServer()->GetPlayer()->SetIsLightsOut(false);

		_noise->SetAlpha(0.f);
		_hud->SetAlpha(0.f);
		_state = STATE::kNone;
		break;
	}
	
	}

	return true;
}