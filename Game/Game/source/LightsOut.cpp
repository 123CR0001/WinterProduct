#include"LightsOut.h"

#include"SpriteTextFlipAnimation.h"

#include"UIHrsMinSec.h"
#include"UI.h"
#include"UISpriteText.h"
#include"UIScreen.h"
#include"UISecMiliSec.h"

#include"ModeGame.h"
#include"ModeColorIn.h"
#include"ModeColorOut.h"

#include"TimeLine.h"
#include"ObjectServer.h"
#include"Player.h"
#include"CommonSoldier.h"

#include"ApplicationMain.h"
#include"ApplicationGlobal.h"

#include"CameraZoomComponent.h"
#include"CreateAfterImageComponent.h"
#include"AnimationComponent.h"
#include"CameraComponent.h"

LightsOut::LightsOut(ModeGame* game) 
	:_game(game)
	,_timerBg(NEW SpriteTextFlipAnimation(3,false))
	,_isUseLightsOut(NEW SpriteTextFlipAnimation(3,true))
	,_timer(NEW UISecMiliSec(Transform2(Vector2(359.f * SCREEN_WIDTH_MAG,978.f * SCREEN_HEIGHT_MAG)),100))
	,_noise(NEW SpriteTextFlipAnimation(8, true))
	,_hud(NEW SpriteText())
	,_afterImageCom(nullptr)
	,_limitCnt(600)
	,_useTimes(1)
	,_state(STATE::kNone)
	,_oldEnergyCount(0)
{

	//タイマーの背景の設定
	_timerBg->Stop();
	_timerBg->LoadDivText("res/UI/Game/Timer/ui_timerbg_01.png", 5, 1, 5, 500, 150);
	_timerBg->SetPos(Vector2(359.f * SCREEN_WIDTH_MAG, 978.f * SCREEN_HEIGHT_MAG));
	_timerBg->SetSize(Vector2(500.f * SCREEN_WIDTH_MAG,150.f* SCREEN_HEIGHT_MAG));

	//ノイズの設定
	_noise->LoadDivText("res/Effect/ui_nightscope_1.png", 6, 1, 6, 1920, 1080);
	_noise->SetSize(Vector2(SCREEN_WIDTH, SCREEN_HEIGHT));
	_noise->SetPos(Vector2(SCREEN_WIDTH / 2.f,  SCREEN_HEIGHT / 2.f));
	_noise->SetAlpha(0.f);

	//HUDの設定
	_hud->SetHandle(ResourceServer::LoadGraph("res/hud_01.png"));
	_hud->SetSize(Vector2(SCREEN_WIDTH, SCREEN_HEIGHT));
	_hud->SetPos(Vector2(SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f));
	_hud->SetAlpha(0.f);

	//ライツアウトの使用回数を表示するUIの設定
	_isUseLightsOut->LoadDivText("res/UI/Game/ui_timerbg_02.png", 5, 1, 5, 200, 200);
	_isUseLightsOut->SetSize(Vector2(200.f * SCREEN_WIDTH_MAG, 200.f * SCREEN_HEIGHT_MAG));
	_isUseLightsOut->SetPos(Vector2(523.f * SCREEN_WIDTH_MAG, 978.f * SCREEN_HEIGHT_MAG));
	_isUseLightsOut->SetAlpha(1.f);

	//描画しない
	_timer->SetIsDraw(false);

	//UIを追加
	_game->GetUIServer()->AddUI(NEW UISpriteText(_noise,300));
	_game->GetUIServer()->AddUI(NEW UISpriteText(_hud,310));
	_game->GetUIServer()->AddUI(NEW UISpriteText(_timerBg,200));
	_game->GetUIServer()->AddUI(NEW UISpriteText(_isUseLightsOut, 250));
	_game->GetUIServer()->AddUI(_timer);
}

LightsOut::~LightsOut(){

}

bool LightsOut::Process() {

	//switch (_state) {
	//case STATE::kNone:
	//	break;
	//case STATE::kStart: {

	//	//タイマーの背景のアニメーションを始める
	//	_timerBg->Play();

	//	//残りの使用回数を減らす
	//	_useTimes--;

	//	//タイマーを30フレーム後に描画されるようにする
	//	auto func = [this]() {
	//		_timer->SetIsDraw(true);
	//	};
	//	_game->GetTimeLine()->AddLine(30, func);

	//	//フェードインアウトを利用した演出
	//	ModeServer::GetInstance()->Add(NEW ModeColorOut(NEW ModeColorIn(10, true), nullptr, 10), 100, "Out");

	//	//ライツアウト中は描画する
	//	_noise->SetAlpha(1.f);
	//	_hud->SetAlpha(1.f);

	//	//プレイヤーから残像を出力するようにする
	//	_afterImageCom = NEW CreateAfterImageComponent(_game->GetObjectServer()->GetPlayer()->GetAnimationComponent());

	//	//Zoomイン
	//	NEW CameraZoomComponent(_game->GetObjectServer()->GetPlayer()->GetCamera(), 0.6f, 60);

	//	//BGMの再生
	//	gGlobal._sndServer.Play("BGM_06");

	//	//敵兵の輪郭線を描画
	//	for (auto&& soldier : _game->GetObjectServer()->GetCommonSoldiers()) {
	//		for (int a = 0; a < MV1GetMaterialNum(soldier->GetHandle()); a++) {
	//			MV1SetMaterialOutLineWidth(soldier->GetHandle(), a, 0.5f);
	//		}
	//	}

	//	//状態をProcessに変更
	//	_state = STATE::kProcess;

	//	break;
	//}

	//case STATE::kProcess: {

	//	//残り時間を減らす
	//	_limitCnt--;
	//	_timer->SetFrameCount(_limitCnt);

	//	//残り時間が0未満になったら、終了処理へ入る
	//	if (_limitCnt < 0) {
	//		_state = STATE::kEnd;
	//	}

	//	//一秒経ったら、SEを流す
	//	if (_limitCnt % 60 == 0) {
	//		//SE
	//		gGlobal._sndServer.Get("SE_15")->Play();
	//	}

	//	break;
	//}
	//case STATE::kEnd: {
	//	
	//	//アニメーションを逆再生させる
	//	_timerBg->Reverse();

	//	//非表示
	//	_timer->SetIsDraw(false);

	//	//フェードインアウトを利用した演出
	//	ModeServer::GetInstance()->Add(NEW ModeColorOut(NEW ModeColorIn(10, true), nullptr, 10), 100, "Out");

	//	//エネミーが残ってたら、ゲームオーバーの演出をする
	//	if(_game->GetEnemyCount() > 0){
	//		SpriteTextFlipAnimation* text = NEW SpriteTextFlipAnimation(10, true);
	//		text->LoadDivText("res/UI/GameOver/ui_sirenfilter_01.png", 6, 1, 6, 1920, 1080);
	//		text->SetSize(Vector2(SCREEN_WIDTH, SCREEN_HEIGHT));
	//		text->SetPos(Vector2(SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f));
	//		_game->GetUIServer()->AddUI(NEW UISpriteText(text));
	//		gGlobal._sndServer.Play("SE_07");
	//	}

	//	//残り使用回数が0だったら
	//	if (_useTimes == 0) {
	//		//120フレーム後にゲームオーバーかゲームクリアを決める
	//		auto func = [this]() {
	//			_game->SwitchOverOrClear();
	//		};

	//		_game->GetTimeLine()->AddLine(120, func);
	//	}

	//	//ライツアウト中は描画しない
	//	_noise->SetAlpha(0.f);
	//	_hud->SetAlpha(0.f);

	//	//Zoomアウト
	//	NEW CameraZoomComponent(_game->GetObjectServer()->GetPlayer()->GetCamera(), 1.f, 60);

	//	//BGMの再生
	//	gGlobal._sndServer.Play("BGM_03");

	//	//敵兵の輪郭線を描画しない
	//	for (auto&& soldier : _game->GetObjectServer()->GetCommonSoldiers()) {
	//		for (int a = 0; a < MV1GetMaterialNum(soldier->GetHandle()); a++) {
	//			MV1SetMaterialOutLineWidth(soldier->GetHandle(), a, 0.f);
	//		}
	//	}

	//	//プレイヤーから削除
	//	if(_afterImageCom) {
	//		_afterImageCom->GetOwner()->DeleteComponent(_afterImageCom);
	//		_afterImageCom = nullptr;
	//	}

	//	_state = STATE::kNone;
	//	break;
	//}

	//default: {
	//	//エラー処理
	//}
	//}

	////ライツアウトが使用可能になったら、UIを表示。そして、SEを流す
	//if(_useTimes > 0 && _game->GetEnergyCount() == 0 && _oldEnergyCount != _game->GetEnergyCount()) {
	//	_isUseLightsOut->SetAlpha(1.f);
	//	gGlobal._sndServer.Play("SE_09");
	//}

	////UIを非表示
	////_oldEnergyCountを更新
	//else {
	//	_isUseLightsOut->SetAlpha(0.f);
	//	_oldEnergyCount = _game->GetEnergyCount();
	//}

	return true;
}