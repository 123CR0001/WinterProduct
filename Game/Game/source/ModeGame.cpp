
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
#include"ModeLightsOut.h"
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

ModeGame::ModeGame(std::string stageNum) 
	:_objServer(NEW ObjectServer(this))
	,_debug(NEW ModeDebugMenu(this))
	,_modeEffekseer(NEW ModeEffekseer())
{
	_stage = stageNum;

	LoadData();
}

ModeGame::~ModeGame() {
	delete _objServer;
	delete _debug;
	delete _uiServer;
	delete _fps;
}

bool ModeGame::Initialize() {
	if (!base::Initialize()) { return false; }


	// アプリケーショングローバルの初期化
	gGlobal.Init();


	_uiServer = NEW UIServer();

	_fps = NEW FPS();

	ModeServer::GetInstance()->Add(_modeEffekseer, 100, MODE_EFFEKSEER_NAME);

	ModeServer::GetInstance()->Add(new ModeMiniMap(this), 100, "MiniMap");

	_debug->Initialize();

	_isCouldLightsOut = false;

	// シャドウマップの生成
	_handleShadowMap = MakeShadowMap(2048, 2048);


	_uiServer->Add(NEW UILightsTimer(), nullptr, 0, 0, 930, 600, 150, 100, "lightsOutTimer");
	// 最初はスライドがいらないので、初期位置と終了位置を同じにしておく
	_uiServer->Add(NEW UIRemainingUses(this, 2, 930, 2, 930,3), nullptr, 0, 2, 930, 450, 150, 100, "remainingUses");
	_cntTest = 0;


	return true;
}

bool ModeGame::Terminate() {

	base::Terminate();

	_debug->Terminate();
	ModeServer::GetInstance()->Del(ModeServer::GetInstance()->Get("MiniMap"));

	return true;
}

bool ModeGame::Process() {
	base::Process();
	// fpsの更新
	_fps->Update();	
	
	_uiServer->Process();

	if (_debug->Process()) { return true; }

	//オブジェクトサーバーの処理
	if (!_objServer->ProcessInit()) { return false; }
	if (!_objServer->Process()) { return false; }

	//LightsOutモードを追加
	if (GetPad()->GetTrgButton() & INPUT_Y  && !ModeServer::GetInstance()->IsAdd("Out") && !ModeServer::GetInstance()->IsAdd("LightsOut")) {

		auto func = [this]() {
			ModeServer::GetInstance()->Add(NEW ModeLightsOut(this), 100, "LightsOut"); 
			//プレイヤーから残像を出力するようにする
			NEW CreateAfterImageComponent(_objServer->GetPlayer()->GetAnimationComponent());

			_isCouldLightsOut = true;
		};

		//フェードイン
		ModeColorIn* colorIn = NEW ModeColorIn(10);
		ModeColorOut* mode = NEW ModeColorOut(colorIn, func, 10);
		ModeServer::GetInstance()->Add(mode, 100, "Out");

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



	if (_isCouldLightsOut && !ModeServer::GetInstance()->IsAdd("LightsOut")) {
		if (_objServer->GetEnemys().size() > 0) {
			ModeServer::GetInstance()->Add(NEW ModeGameOver(), 100, "GameOver");
		}
		else {
			ModeServer::GetInstance()->Add(NEW ModeClear(), 100, "GameClear");
		}
	}

	if (GetPad()->GetTrgButton() & INPUT_DPAD_UP) {
		_cntTest++;
	}

	if (GetPad()->GetTrgButton() & INPUT_X) {
		_uiServer->Search("remainingUses")->Selected();
		_uiServer->Search("lightsOutTimer")->Selected();
	}

	// fpsの待機
	_fps->WaitFPS();
	auto CommonSoldiers = GetObjectServer()->GetCommonSoldiers();
	if (CommonSoldiers.size() == 0) { _fps->SetFPS(50); }

	return true;
}

bool ModeGame::Render() {
	base::Render();
	// 3D基本設定
	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(TRUE);
	SetUseBackCulling(TRUE);



#if 0	// 平行ライト
	SetGlobalAmbientLight(GetColorF(0.5f, 0.f, 0.f, 0.f));
	ChangeLightTypeDir(VGet(-1, -1, 0));
#endif

#if 1	// ポイントライト
	VECTOR vLightDir = VGet(-1, -1, 0.5f);
	// 平行ライト
	SetGlobalAmbientLight(GetColorF(0.f, 0.f, 0.f, 0.f));
	ChangeLightTypeDir(vLightDir);
	
#endif

	// シャドウマップが想定するライトの方向もセット
	SetShadowMapLightDirection(_handleShadowMap, vLightDir);

	// シャドウマップに描画する範囲を設定
	SetShadowMapDrawArea(_handleShadowMap, VGet(-500.0f, -1.0f, -500.0f), VGet(500.0f, 500.0f, 500.0f));

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
	if (!_objServer->Renderer()) { return false; }
	
	DrawFormatString(0, 16, GetColor(255, 0, 0), "stage%s", _stage);

	_uiServer->Render();

	_debug->Render();

	return true;
}

bool ModeGame::LoadData() {
	std::string stage = _stage.substr(0, 1);
	std::string area = _stage.substr(2, 1);
	nlohmann::json j;
	std::string str = "res/stage/stage" + stage + "/" + area + "/StageData.json";
	std::ifstream file(str);

	if (!file) { return false; }
	file >> j;

	//プレイヤーの読み込み
	for (auto&& object : j.at("player")) {
		std::string name = object.at("objectName");
		if (name == "marker1") {
			Player* p = NEW Player(GetObjectServer());
			p->SetJsonDataUE(object);
		}

	}
	
	//エネミーの読み込み
	int count = 1;
	char num[10];

	std::string enemyName = "enemy_file/commonsoldier";

	while (1) {

		snprintf(num, 8, "%d", count);
		enemyName += num;

		if (j.find(enemyName.c_str()) != j.end()) {

			ObjectBase* p = NEW CommonSoldier(GetObjectServer());
			p->SetJsonDataUE(j.at(enemyName.c_str()));

			enemyName = "enemy_file/commonsoldier";
			count++;

		}
		else {
			break;
		}
	}

	struct ModelData {
		const char* filePath;
		const char* attachFrameName;
		std::function < ObjectBase* (const char*,const char*) > func;
	};

	std::function < ObjectBase* ( const char*, const char*) > func =
		[this]( const char* path, const char* frameName) {
		ObjectBase* p = NEW ObjectBase(GetObjectServer(), true);
		p->LoadModel(path, frameName);
		MV1RefreshCollInfo(p->GetHandle(), p->GetAttachIndex());
		return p;
	};

	std::unordered_map<std::string, ModelData>map;

	std::string strPath = "res/stage/stage" + stage + "/" + area + "/";

	std::string filePath = strPath + "model/StageObject.mv1";
	std::string attachFrameName = "UCX_StageObject";
	map["StageObject"].filePath = filePath.c_str();
	map["StageObject"].attachFrameName = attachFrameName.c_str();

	std::string filePathColl = strPath +"NavigationMesh.mv1";
	std::string attachFrameNameColl = "NavigationMesh";
	map["NavigationMesh"].filePath = filePathColl.c_str();
	map["NavigationMesh"].attachFrameName = attachFrameNameColl.c_str();

	map["siren"].filePath = "res/Object/siren/siren.mv1";
	map["siren"].attachFrameName = "UCX_siren1";
	map["siren"].func = [this](const char* path, const char* frameName) {return NEW Siren(GetObjectServer()); };

	{
		//ナビゲーション
		if (j.find("navmesh") != j.end()) {
			for (auto&& navMesh : j.at("navmesh")) {
				std::string name = navMesh.at("objectName");
				if (map.find(name) != map.end()) {
					_objServer->SetNavigationModel(map[name].filePath, map[name].attachFrameName);
					MV1SetPosition(_objServer->GetNavigationHandle(), VGet(navMesh.at("translate").at("x"), navMesh.at("translate").at("z"), -1 * navMesh.at("translate").at("y")));
					MV1SetRotationXYZ(_objServer->GetNavigationHandle(), VGet(DegToRad(90.f), 0.f, 0.f));
					MV1RefreshCollInfo(_objServer->GetNavigationHandle(), _objServer->GetNavigationAttachIndex());
				}
			}
		}
	}

	//Spawner
	NEW TracerSpawner(_objServer);

	//オブジェクトの配置
	for (auto&& object : j.at("object")) {
		std::string name = object.at("objectName");

		//オブジェクト名＋数字		例：Enemy1
		//数字部分を削除

		//0~9の数字が含まれていれば、それ以降の文字を削除
		for(int a = 0; a < 10; a++) {
			//整数をstringに変換　数字を検索
			int num = name.find(std::to_string(a));

			//findは、検索した文字がなければ、-1を返す
			if(num != -1) { name = name.substr(0, num); break; }
		}
	
		if (map.find(name) != map.end()) {
				ObjectBase* p = NEW ObjectBase(GetObjectServer(),true);
				p->LoadModel(map[name].filePath, map[name].attachFrameName);
				p->SetJsonDataUE(object);
				p->AddEulerAngle(Vector3D(DegToRad(90.f), DegToRad(180.f), 0.f));
				MV1RefreshCollInfo(p->GetHandle(), p->GetAttachIndex());
		}
	}


	return true;
}

XGamePad* ModeGame::GetPad()const { return ApplicationMain::GetInstance()->GetPad(); }

