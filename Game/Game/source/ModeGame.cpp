
#include "AppFrame.h"
#include<fstream>
#include<unordered_map>
#include "ApplicationMain.h"
#include"ApplicationGlobal.h"

#include "ModeGame.h"
#include"ModeEffekseer.h"
#include"ModeDebugMenu.h"
#include"ModeColorOut.h"
#include"ModeColorIn.h"
#include"ModePause.h"
#include"ModeGameOver.h"
#include"ModeClear.h"
#include"ModeMiniMap.h"

#include"ObjectServer.h"
#include"CommonSoldier.h"
#include"Player.h"
#include"Siren.h"
#include"Tracer.h"
#include"TracerSpawner.h"

#include"SoundComponent.h"
#include"PhysWorld.h"

#include"CreateAfterImageComponent.h"
#include"FPS.h"
#include"AnimationComponent.h"

#include"UIServer.h"
#include"UILightsTimer.h"
#include "UIRemainingUses.h"

#include"FPS.h"
#include"Timer.h"

#include"CameraZoomComponent.h"
#include"CameraComponent.h"

#include"TimeLine.h"
#include"MyUIServer.h"
#include"LightsOut.h"

#include"UIDetectionLevel.h"

ModeGame::ModeGame(std::string stageNum) 
	:_objServer(NEW ObjectServer(this))
	,_debug(NEW ModeDebugMenu(this))
	,_modeEffekseer(NEW ModeEffekseer())
	,_stage(stageNum)
	,_energyCount(0)
	,_resultData(std::make_shared<ResultData>())
	,_timeLine(NEW TimeLine())
	,_uiServer(NEW MyUIServer())
	,_enemyCount(0)
{
	_fps = NEW FPS();


	ModeServer::GetInstance()->Add(_modeEffekseer, 100, MODE_EFFEKSEER_NAME);
	_debug->Initialize();
}

ModeGame::~ModeGame() {
	delete _objServer;
	delete _debug;
	delete _fps;
	delete _uiServer;
	delete _lightsOut;
	delete _timeLine;
}

bool ModeGame::Initialize() {
	if (!base::Initialize()) { return false; }


	_objServer->LoadData(_stage);

	ModeServer::GetInstance()->Add(NEW ModeMiniMap(this), 100, "MiniMap");

	// シャドウマップの生成
	_handleShadowMap = MakeShadowMap(2048, 2048);


	_bg = ResourceServer::LoadGraph("res/gamemain_bg.png");

	_lightsOut = NEW LightsOut(this);

	_uiServer->AddUI(NEW UIDetectionLevel(_objServer));
	return true;
}

bool ModeGame::Terminate() {

	base::Terminate();

	DeleteShadowMap(_handleShadowMap);
	_objServer->Terminate();
	ModeServer::GetInstance()->Del(ModeServer::GetInstance()->Get("MiniMap"));

	return true;
}

bool ModeGame::Process() {
	base::Process();

	_timeLine->Process();
	_uiServer->Process();
	_lightsOut->Process();

	// fpsの更新
	_fps->Update();	
	

	if (_debug->Process()) { return true; }

	//オブジェクトサーバーの処理
	if (!_objServer->ProcessInit()) { return false; }
	if (!_objServer->Process()) { return false; }

	//LightsOutモードを追加
	if (_lightsOut->IsUse()
		&& GetPad()->GetTrgButton() & INPUT_Y  
		&& _energyCount == 0
		) {

		gGlobal._sndServer.Get("SE_09")->Play();

		//プレイヤーから残像を出力するようにする
		NEW CreateAfterImageComponent(_objServer->GetPlayer()->GetAnimationComponent());
		NEW CameraZoomComponent(_objServer->GetPlayer()->GetCamera(), -0.3f, 60);
	
		_lightsOut->Use();
	}

	//Pauseモードを追加
	if (GetPad()->GetTrgButton() & INPUT_START && !ModeServer::GetInstance()->IsAdd("Pause")) {
		ModeServer::GetInstance()->Add(NEW ModePause(), 100, "Pause");

	}

	//勝敗判定
	for (auto iter = _objServer->GetCommonSoldiers().begin(); iter != _objServer->GetCommonSoldiers().end(); ++iter) {
		//検知度が一定を超える敵がいたら、ゲームオーバー
		if ((*iter)->GetDetectionLevel() >= 1.f) {

			//ModeServer::GetInstance()->Add(NEW ModeGameOver(), 100, "GameOver");

		}
	}

	return true;
}

bool ModeGame::Render() {
	base::Render();
	// 3D基本設定
	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(TRUE);
	SetUseBackCulling(TRUE);

	auto instance = ApplicationBase::GetInstance();

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
#if 0	// 平行ライト
	SetGlobalAmbientLight(GetColorF(0.f, 0.f, 0.f, 0.f));
	ChangeLightTypeDir(vLightDir);
#endif
#if 1	// ポイントライト
	SetGlobalAmbientLight(GetColorF(0.f, 0.f, 0.f, 0.f));
	ChangeLightTypePoint(VAdd(target, VGet(0, 50.f, 0)), 1000.f, 0.f, 0.005f, 0.f);
#endif

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

	_uiServer->Draw();
	_debug->Render();

	return true;
}

XGamePad* ModeGame::GetPad()const { return ApplicationMain::GetInstance()->GetPad(); }

void ModeGame::SwitchOverOrClear() {
	if (_enemyCount == 0) {
		auto func = [this]() {
			// モードの削除
			ModeServer::GetInstance()->Del(ModeServer::GetInstance()->Get("ui"));
			ModeServer::GetInstance()->Del(ModeServer::GetInstance()->Get(MODE_EFFEKSEER_NAME));
			ModeServer::GetInstance()->Del(this);
			// 次のモードを登録
			ModeServer::GetInstance()->Add(NEW ModeClear(_resultData), 100, "GameClear");
		};
		// 次のモードを登録
		ModeColorIn* colorIn = NEW ModeColorIn(60, true);
		ModeBase* mode = NEW ModeColorOut(colorIn, func);
		ModeServer::GetInstance()->Add(mode, 100, "Out");
	}
	else {
		ModeServer::GetInstance()->Del(ModeServer::GetInstance()->Get("ui"));
		ModeServer::GetInstance()->Del(ModeServer::GetInstance()->Get(MODE_EFFEKSEER_NAME));
		ModeServer::GetInstance()->Del(this);

		ModeServer::GetInstance()->Add(NEW ModeGameOver(this), 100, "GameOver");
	}
}
