
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

SoundComponent* m = nullptr;

bool ModeGame::Initialize() {
	if (!base::Initialize()) { return false; }


	// アプリケーショングローバルの初期化
	gGlobal.Init();

	_objServer = new ObjectServer(this);

	LoadData();

	_modeEffekseer = new ModeEffekseer();

	ModeServer::GetInstance()->Add(_modeEffekseer, 100, MODE_EFFEKSEER_NAME);
	ModeServer::GetInstance()->Add(new ModeDebugMenu(this), 300, "Debug");

	return true;
}

bool ModeGame::Terminate() {

	base::Terminate();

	delete _objServer;

	return true;
}

bool ModeGame::Process() {
	base::Process();

	if (!_objServer->ProcessInit()) { return false; }
	if (!_objServer->Process()) { return false; }
	if (GetPad()->GetTrgButton() & INPUT_Y  && !ModeServer::GetInstance()->IsAdd("Out")) {
		ModeColorIn* colorIn = new ModeColorIn(10);
		ModeColorOut* mode = new ModeColorOut(colorIn,nullptr, 10, new ModeLightsOut(), 100, "LightsOut");
		ModeServer::GetInstance()->Add(mode, 100, "Out");
	}
	if (GetPad()->GetTrgButton() & INPUT_START && !ModeServer::GetInstance()->IsAdd("Pause")) {
		ModeServer::GetInstance()->Add(new ModePause(), 100, "Pause");
	}

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

	return true;
}

bool ModeGame::LoadData() {
	nlohmann::json j;
	std::ifstream file("res/map/stage1.json");

	if (!file) { return false; }
	file >> j;

	//プレイヤーの読み込み
	for (auto&& object : j.at("player")) {
		std::string name = object.at("objectName");
		if (name == "marker1") {
			Player* p = new Player(GetObjectServer());
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

			ObjectBase* p = new CommonSoldier(GetObjectServer());
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
	map["stage1"].filePath = "res/Stage/stage1/stage1.mv1";
	map["stage1"].attachFrameName = "UCX_stage1";

	map["mapcollisionstage1"].filePath = "res/Map/mapcollisionstage1.mv1";
	map["mapcollisionstage1"].attachFrameName = "mapcollisionstage1";

	{
		//ナビゲーション
		if (j.find("mapcollision") != j.end()) {
			for (auto mapcollision : j.at("mapcollision")) {
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
				ObjectBase* p = new ObjectBase(GetObjectServer());
				p->LoadModel(map[name].filePath, map[name].attachFrameName);
				p->SetJsonDataUE(object);
				p->AddEulerAngle(Vector3D(DegToRad(90.f), DegToRad(180.f), 0.f));
				MV1RefreshCollInfo(p->GetHandle(), p->GetAttachIndex());
		}
	}

	return true;
}

XGamePad* ModeGame::GetPad()const { return ApplicationMain::GetInstance()->GetPad(); }

