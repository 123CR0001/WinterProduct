
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

#include"ObjectServer.h"
#include"CommonSoldier.h"
#include"Player.h"
#include"Siren.h"

#include"SoundComponent.h"
#include"PhysWorld.h"

#include"CreateAfterImageComponent.h"

SoundComponent* m = nullptr;

bool ModeGame::Initialize() {
	if (!base::Initialize()) { return false; }


	// �A�v���P�[�V�����O���[�o���̏�����
	gGlobal.Init();

	_objServer = NEW ObjectServer(this);

	LoadData();

	_modeEffekseer = NEW ModeEffekseer();

	ModeServer::GetInstance()->Add(_modeEffekseer, 100, MODE_EFFEKSEER_NAME);
	ModeServer::GetInstance()->Add(NEW ModeDebugMenu(this), 300, "Debug");

	_isCouldLightsOut = false;
	return true;
}

bool ModeGame::Terminate() {

	base::Terminate();

	delete _objServer;

	return true;
}

bool ModeGame::Process() {
	base::Process();

	//�I�u�W�F�N�g�T�[�o�[�̏���
	if (!_objServer->ProcessInit()) { return false; }
	if (!_objServer->Process()) { return false; }

	//LightsOut���[�h��ǉ�
	if (GetPad()->GetTrgButton() & INPUT_Y  && !ModeServer::GetInstance()->IsAdd("Out") && !ModeServer::GetInstance()->IsAdd("LightsOut")) {

		auto func = [this]() {
			ModeServer::GetInstance()->Add(NEW ModeLightsOut(this), 100, "LightsOut"); 
			//�v���C���[����c�����o�͂���悤�ɂ���
			NEW CreateAfterImageComponent(_objServer->GetPlayer());

			_isCouldLightsOut = true;
		};

		//�t�F�[�h�C��
		ModeColorIn* colorIn = NEW ModeColorIn(10);
		ModeColorOut* mode = NEW ModeColorOut(colorIn, func, 10);
		ModeServer::GetInstance()->Add(mode, 100, "Out");

	}

	//Pause���[�h��ǉ�
	if (GetPad()->GetTrgButton() & INPUT_START && !ModeServer::GetInstance()->IsAdd("Pause")) {
		ModeServer::GetInstance()->Add(NEW ModePause(), 100, "Pause");

	}

	//���s����
	for (auto iter = _objServer->GetCommonSoldiers().begin(); iter != _objServer->GetCommonSoldiers().end(); ++iter) {
		if ((*iter)->GetDetectionLevel() >= 1.f) {

			//ModeServer::GetInstance()->Add(NEW ModeGameOver(), 100, "GameOver");

		}
	}

	if (_isCouldLightsOut && !ModeServer::GetInstance()->IsAdd("LightsOut")) {
		if (_objServer->GetCommonSoldiers().size() > 0) {
			ModeServer::GetInstance()->Add(NEW ModeGameOver(), 100, "GameOver");
		}
		else {
			ModeServer::GetInstance()->Add(NEW ModeClear(), 100, "GameClear");
		}
	}

	return true;
}

bool ModeGame::Render() {
	base::Render();
	// 3D��{�ݒ�
	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(TRUE);
	SetUseBackCulling(TRUE);

#if 0	// ���s���C�g
	SetGlobalAmbientLight(GetColorF(0.5f, 0.f, 0.f, 0.f));
	ChangeLightTypeDir(VGet(-1, -1, 0));
#endif
#if 1	// �|�C���g���C�g
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

	//�v���C���[�̓ǂݍ���
	for (auto&& object : j.at("player")) {
		std::string name = object.at("objectName");
		if (name == "marker1") {
			Player* p = NEW Player(GetObjectServer());
			p->SetJsonDataUE(object);
		}
	}
	
	//�G�l�~�[�̓ǂݍ���
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

	map["stage1"].filePath = "res/Stage/stage1/stage1.mv1";
	map["stage1"].attachFrameName = "UCX_stage1";
	map["stage1"].func = func;

	map["siren"].filePath = "res/Object/siren/siren.mv1";
	map["siren"].attachFrameName = "UCX_siren1";
	map["siren"].func = [this](const char* path,const char* frameName) {return NEW Siren(GetObjectServer()); };

	map["mapcollisionstage1"].filePath = "res/Map/mapcollisionstage1.mv1";
	map["mapcollisionstage1"].attachFrameName = "mapcollisionstage1";

	for (auto&& object : j.at("object")) {
		std::string name = object.at("objectName");

		auto iter = map.begin();

		//�o�^���Ă��閼�O�����ɂ�������A���̉�����
		for (iter; iter != map.end(); ++iter) {

			number = iter->first.find(name);

			if (number == 0) {
				ObjectBase* p = iter->second.func(iter->second.filePath, iter->second.attachFrameName);
				p->SetJsonDataUE(object);
				p->AddEulerAngle(Vector3D(DegToRad(90.f), DegToRad(180.f), 0.f));
				break;
			}
		}
	}


	{
		//�i�r�Q�[�V����
		if (j.find("mapcollision") != j.end()) {
			for (auto&& mapcollision : j.at("mapcollision")) {
				std::string name = mapcollision.at("objectName");
				if (map.find(name) != map.end()) {
					_objServer->SetNavigationModel(map[name].filePath, map[name].attachFrameName);
					MV1SetPosition(_objServer->GetNavigationHandle(), VGet(mapcollision.at("translate").at("x"), mapcollision.at("translate").at("z"), -1 * mapcollision.at("translate").at("y")));
					MV1SetRotationXYZ(_objServer->GetNavigationHandle(), VGet(DegToRad(90.f), 0.f, 0.f));
					MV1RefreshCollInfo(_objServer->GetNavigationHandle(), _objServer->GetNavigationAttachIndex());
				}
			}
		}
	}


	return true;
}

XGamePad* ModeGame::GetPad()const { return ApplicationMain::GetInstance()->GetPad(); }

