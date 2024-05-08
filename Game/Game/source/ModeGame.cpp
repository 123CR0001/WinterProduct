
#include "AppFrame.h"
#include<fstream>
#include<unordered_map>
#include "ApplicationMain.h"
#include"ApplicationGlobal.h"

#include "ModeGame.h"
#include"ModeEffekseer.h"
#include"ModeColorOut.h"
#include"ModeColorIn.h"
#include"ModePause.h"
#include"ModeGameOver.h"
#include"ModeClear.h"

#include"ObjectServer.h"
#include"CommonSoldier.h"
#include"Player.h"
#include"Siren.h"
#include"Tracer.h"
#include"TracerSpawner.h"

#include"SoundComponent.h"
#include"PhysWorld.h"

#include"CreateAfterImageComponent.h"
#include"AnimationComponent.h"

#include"UIServer.h"

#include"CameraZoomComponent.h"
#include"CameraComponent.h"
#include"MotionComponent.h"

#include"TimeLine.h"

#include"UIScreen.h"
#include"LightsOutComponent.h"

#include"UIDetectionLevel.h"
#include"UIVision.h"
#include"UIMiniMap.h"
#include"SpriteNumber.h"
#include"SpriteTextFlipAnimation.h"
#include"UISpriteText.h"
#include"SoundItem.h"

#include"ClearData.h"

ModeGame::ModeGame(std::string stageNum)
	:_objServer(NEW ObjectServer(this))
	, _modeEffekseer(NEW ModeEffekseer())
	, _stage(stageNum)
	, _energyCount(0)
	, _resultData(std::make_shared<ClearData>())
	, _timeLine(NEW TimeLine())
	, _UIScreen(NEW UIScreen())
	, _enemyCount(0)
	, _enemyCountText(NEW SpriteNumber(_enemyCount, 2))
	, _isUsingLightsOut(false)
{


	ModeServer::GetInstance()->Add(_modeEffekseer, 100, MODE_EFFEKSEER_NAME);

	_resultData->_stageName = stageNum;
}

ModeGame::~ModeGame() {
	delete _objServer;
	delete _UIScreen;
	delete _timeLine;
}

bool ModeGame::Initialize() {
	if (!base::Initialize()) { return false; }

	//ステージの読み込み
	_objServer->LoadData(_stage);

	// シャドウマップの生成
	_handleShadowMap = MakeShadowMap(2048, 2048);

	//背景
	_bg = ResourceServer::LoadGraph("res/gamemain_bg.png");

	//残りの敵の数を描画で使用する画像の設定
	_enemyCountText->LoadDivNumber("res/UI/Result/ui_timer_01.png", 5, 2, 46, 70);
	_enemyCountText->SetPos(Vector2(365.f * SCREEN_WIDTH_MAG,858.f*SCREEN_HEIGHT_MAG));
	_enemyCountText->SetSize(Vector2(46.f * SCREEN_WIDTH_MAG, 70.f * SCREEN_HEIGHT_MAG));
	_enemyCountText->SetAlpha(0.f);

	//_enemyCountTextの背景
	{
		_enemyCountBg = NEW SpriteText(
			ResourceServer::LoadGraph("res/UI/Game/CommonSoldierTimes.png"),
			Transform2(Vector2(224.f * SCREEN_WIDTH_MAG, 847.f * SCREEN_HEIGHT_MAG)),
			Vector2(160.f * SCREEN_WIDTH_MAG, 120.f * SCREEN_HEIGHT_MAG)
		);
		_enemyCountBg->SetAlpha(0.f);
		_UIScreen->AddUI(NEW UISpriteText(_enemyCountBg,100));
	}

	//UIの追加
	_UIScreen->AddUI(NEW UISpriteText(_enemyCountText, 50));
	_UIScreen->AddUI(NEW UIDetectionLevel(_objServer,100));
	_UIScreen->AddUI(NEW UIVision(_objServer,1000));
	_UIScreen->AddUI(NEW UIMiniMap(this,100));


	gGlobal._sndServer.Play("BGM_03");
	return true;
}

bool ModeGame::Terminate() {

	base::Terminate();

	// シャドウマップの削除
	DeleteShadowMap(_handleShadowMap);

	//オブジェクトサーバーの終了処理
	_objServer->Terminate();

	//BGMを止める
	gGlobal._sndServer.StopType(SoundItemBase::TYPE::BGM);

	return true;
}

bool ModeGame::Process() {
	base::Process();

	//更新
	_timeLine->Process();
	_UIScreen->Process();

	if (!_objServer->Process()) { return false; }

	//ライツアウトが使用中なら、一部のUIの表示する
	if (_isUsingLightsOut) {
		if(_enemyCountBg->GetAlpha() != 1.f)_enemyCountBg->SetAlpha(1.f);
		if(_enemyCountText->GetAlpha() != 1.f)_enemyCountText->SetAlpha(1.f);
	}

	//描画する数字の更新
	_enemyCountText->SetNumber(_enemyCount);

	return true;
}

bool ModeGame::Render() {
	base::Render();
	// 3D基本設定
	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(TRUE);
	SetUseBackCulling(TRUE);

	auto instance = ApplicationBase::GetInstance();

	// 背景描画
	DrawModiGraph(
		0, 0,
		instance->DispSizeW(),0,
		instance->DispSizeW(), instance->DispSizeH(),
		0,instance->DispSizeH(),
		_bg,
		TRUE
	);

	// ライト設定
	SetUseLighting(TRUE);

	VECTOR vLightDir = VGet(-1, -1, 0.5f);
	VECTOR target = _objServer->GetPlayer()->GetDxPos();

	// ポイントライト
	SetGlobalAmbientLight(GetColorF(0.f, 0.f, 0.f, 0.f));
	ChangeLightTypePoint(VAdd(target, VGet(0, 50.f, 0)), 1000.f, 0.f, 0.005f, 0.f);

	// シャドウマップが想定するライトの方向もセット
	SetShadowMapLightDirection(_handleShadowMap, vLightDir);

	// シャドウマップに描画する範囲を設定
	target.y += 50.f;

	SetShadowMapDrawArea(_handleShadowMap,VAdd(target, VGet(-800.0f, -1.0f, -800.0f)), VAdd(target, VGet(800.0f, 800.0f, 800.0f)));

	// 2回まわして、path=0:シャドウマップへの描画, path=1:モデルの描画
	for (int path = 0; path < 2; path++) {
		if (path == 0) {
			// シャドウマップへの描画の準備
			ShadowMap_DrawSetup(_handleShadowMap);
		}
		else if (path == 1) {
			// シャドウマップへの描画を終了
			ShadowMap_DrawEnd();
			// 描画に使用するシャドウマップを設定
			SetUseShadowMap(0, _handleShadowMap);
		}

		if (!_objServer->Renderer()) { return false; }
	}

	// 描画に使用するシャドウマップの設定を解除
	SetUseShadowMap(0, -1);

	//UIの描画
	_UIScreen->Draw();

	return true;
}

XGamePad* ModeGame::GetPad()const { return ApplicationMain::GetInstance()->GetPad(); }

void ModeGame::SwitchOverOrClear() {
	//敵の数が0ならクリア
	if(_enemyCount == 0) {
		{
			
			_objServer->GetPlayer()->ChangeState("Clear");
			NEW CameraZoomComponent(_objServer->GetPlayer()->GetCamera(),0.5f,60);
		}
		//クリア画面の遷移
		{
			auto timeLineFunc = [=]()mutable {
				auto func = [this]() {
					// モードの削除	
					ModeServer::GetInstance()->Del(ModeServer::GetInstance()->Get(MODE_EFFEKSEER_NAME));
					ModeServer::GetInstance()->Del(this);
					// 次のモードを登録
					ModeServer::GetInstance()->Add(NEW ModeClear(_resultData), 100, "GameClear");
					};
				// 次のモードを登録
				ModeColorIn* colorIn = NEW ModeColorIn(60, true);
				ModeBase* mode = NEW ModeColorOut(colorIn, func);
				ModeServer::GetInstance()->Add(mode, 100, "Out");
				};
			_timeLine->AddLine(180, timeLineFunc); 
		}
	}
	//敵の数が0でないならゲームオーバー
	else {
		{
			_objServer->GetPlayer()->ChangeState("Dead");
		}

		{

			auto timeLineFunc = [=]()mutable {
				auto func = [this]() {
					ModeServer::GetInstance()->Del(ModeServer::GetInstance()->Get(MODE_EFFEKSEER_NAME));
					ModeServer::GetInstance()->Del(this);
					ModeServer::GetInstance()->Add(NEW ModeGameOver(this), 100, "GameOver");
					};
				// 次のモードを登録
				ModeColorIn* colorIn = NEW ModeColorIn(60, true);
				ModeBase* mode = NEW ModeColorOut(colorIn, func);
				ModeServer::GetInstance()->Add(mode, 100, "Out");
				};
			_timeLine->AddLine(180, timeLineFunc); 
		}
	}
}

void ModeGame::DecrementEnemyCount() {
	//敵の数を減らす
	_enemyCount--;

	//敵の数が0ならクリア処理を行う
	if(_enemyCount == 0) {

		//60フレーム後にクリア処理を行う
		_timeLine->AddLine(60, [=]()mutable { SwitchOverOrClear(); });
	}
}