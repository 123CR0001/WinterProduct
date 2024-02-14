
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

#include"ObjectServer.h"
#include"CommonSoldier.h"
#include"Player.h"
#include"Siren.h"

#include"SoundComponent.h"
#include"PhysWorld.h"

#include"FPS.h"

SoundComponent* m = nullptr;

ModeGame::ModeGame(std::string stageNum) {
	_stage = stageNum;
}

bool ModeGame::Initialize() {
	if (!base::Initialize()) { return false; }


	// アプリケーショングローバルの初期化
	gGlobal.Init();

	_objServer = NEW ObjectServer(this);

	LoadData();

	_modeEffekseer = NEW ModeEffekseer();

	_fps = NEW FPS();

	ModeServer::GetInstance()->Add(_modeEffekseer, 100, MODE_EFFEKSEER_NAME);
	ModeServer::GetInstance()->Add(NEW ModeDebugMenu(this), 300, "Debug");
	
	return true;
}

bool ModeGame::Terminate() {

	base::Terminate();

	delete _objServer;

	return true;
}

bool ModeGame::Process() {
	base::Process();
	// fpsの更新
	_fps->Update();	

	if (!_objServer->ProcessInit()) { return false; }
	if (!_objServer->Process()) { return false; }
	if (GetPad()->GetTrgButton() & INPUT_Y  && !ModeServer::GetInstance()->IsAdd("Out")) {
		ModeColorIn* colorIn = NEW ModeColorIn(10);
		ModeColorOut* mode = NEW ModeColorOut(colorIn,nullptr, 10, NEW ModeLightsOut(), 100, "LightsOut");
		ModeServer::GetInstance()->Add(mode, 100, "Out");
	}
	if (GetPad()->GetTrgButton() & INPUT_START && !ModeServer::GetInstance()->IsAdd("Pause")) {
		ModeServer::GetInstance()->Add(NEW ModePause(), 100, "Pause");
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
	SetGlobalAmbientLight(GetColorF(0.f, 0.f, 0.f, 0.f));
	
#endif

	if (!_objServer->Renderer()) { return false; }

	DrawFormatString(0, 16, GetColor(255, 0, 0), "stage%s", _stage);

	return true;
}

bool ModeGame::LoadData() {
	std::string stage = _stage.substr(0, 1);
	std::string area = _stage.substr(2, 1);
	nlohmann::json j;
	std::string str = "res/map/stage" + stage + "/" + area + "/stage" + area + ".json";
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
	};

	std::unordered_map<std::string, ModelData>map;
	std::string filePath = "res/Stage/stage" + stage + "/stage" + stage + ".mv1";
	std::string attachFrameName = "UCX_stage" + stage;
	map["stage" + stage].filePath = filePath.c_str();
	map["stage" + stage].attachFrameName = attachFrameName.c_str();

	std::string filePathColl = "res/Map/stage" +  stage + "/" + area +"/mapcollisionstage" + stage + ".mv1";
	std::string attachFrameNameColl = "mapcollisionstage" + stage;
	map["mapcollisionstage" + stage].filePath = filePathColl.c_str();
	map["mapcollisionstage" + stage].attachFrameName = attachFrameNameColl.c_str();

	{
		//ナビゲーション
		if (j.find("mapcollision") != j.end()) {
			for (auto&& mapcollision : j.at("mapcollision")) {
				std::string name = mapcollision.at("objectName");
				if (map.find(name) != map.end()) {
					_objServer->SetNavigationModel(map[name].filePath, map[name].attachFrameName);
					MV1SetPosition(_objServer->GetNavigationHandle(),VGet(mapcollision.at("translate").at("x"),mapcollision.at("translate").at("z"),-1* mapcollision.at("translate").at("y")));
					MV1SetRotationXYZ(_objServer->GetNavigationHandle(),VGet(DegToRad(90.f), 0.f, 0.f));
					MV1RefreshCollInfo(_objServer->GetNavigationHandle(), _objServer->GetNavigationAttachIndex());
					
				}
			}
		}
	}
	
	for (auto&& object : j.at("object")) {
		std::string name = object.at("objectName");
	
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

