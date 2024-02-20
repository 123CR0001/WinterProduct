
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
#include"Traser.h"
#include"TraserSpawner.h"

#include"SoundComponent.h"
#include"PhysWorld.h"

#include"CreateAfterImageComponent.h"
#include"FPS.h"
#include"AnimationComponent.h"

SoundComponent* m = nullptr;

ModeGame::ModeGame(std::string stageNum) {
	_stageNum = stageNum;
}

ModeGame::~ModeGame() {
	
	delete _objServer;
	delete _debug;
}

bool ModeGame::Initialize() {
	if (!base::Initialize()) { return false; }


	// �A�v���P�[�V�����O���[�o���̏�����
	gGlobal.Init();

	_objServer = NEW ObjectServer(this);



	_modeEffekseer = NEW ModeEffekseer();

	_fps = NEW FPS();

	ModeServer::GetInstance()->Add(_modeEffekseer, 100, MODE_EFFEKSEER_NAME);

	_debug = NEW ModeDebugMenu(this);
	_debug->Initialize();

	_isCouldLightsOut = false;

	// �V���h�E�}�b�v�̐���
	_handleShadowMap = MakeShadowMap(2048, 2048);

	LoadData();

	return true;
}

bool ModeGame::Terminate() {

	base::Terminate();
	_debug->Terminate();

	return true;
}

bool ModeGame::Process() {
	base::Process();
	// fps�̍X�V
	_fps->Update();	

	if (_debug->Process()) { return true; }

	//�I�u�W�F�N�g�T�[�o�[�̏���
	if (!_objServer->ProcessInit()) { return false; }
	if (!_objServer->Process()) { return false; }

	//LightsOut���[�h��ǉ�
	if (GetPad()->GetTrgButton() & INPUT_Y  && !ModeServer::GetInstance()->IsAdd("Out") && !ModeServer::GetInstance()->IsAdd("LightsOut")) {

		auto func = [this]() {
			ModeServer::GetInstance()->Add(NEW ModeLightsOut(this), 100, "LightsOut"); 
			//�v���C���[����c�����o�͂���悤�ɂ���
			NEW CreateAfterImageComponent(_objServer->GetPlayer()->GetAnimationComponent());

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
		//���m�x�����𒴂���G��������A�Q�[���I�[�o�[
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

	// fps�̑ҋ@
	_fps->WaitFPS();
	auto CommonSoldiers = GetObjectServer()->GetCommonSoldiers();
	if (CommonSoldiers.size() == 0) { _fps->SetFPS(50); }

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
	VECTOR vLightDir = VGet(-1, -1, 0.5f);
	// ���s���C�g
	SetGlobalAmbientLight(GetColorF(0.f, 0.f, 0.f, 0.f));
	ChangeLightTypeDir(vLightDir);
	
#endif

	// �V���h�E�}�b�v���z�肷�郉�C�g�̕������Z�b�g
	SetShadowMapLightDirection(_handleShadowMap, vLightDir);

	// �V���h�E�}�b�v�ɕ`�悷��͈͂�ݒ�
	SetShadowMapDrawArea(_handleShadowMap, VGet(-500.0f, -1.0f, -500.0f), VGet(500.0f, 500.0f, 500.0f));

	// 2��܂킵�āApath=0:�V���h�E�}�b�v�ւ̕`��, path=1:���f���̕`��
	for (int path = 0; path < 2; path++) {
		if (path == 0) {
			// �V���h�E�}�b�v�ւ̕`��̏���
			ShadowMap_DrawSetup(_handleShadowMap);
		}
		else if (path == 1) {
			// �V���h�E�}�b�v�ւ̕`����I��
			ShadowMap_DrawEnd();
			// �`��Ɏg�p����V���h�E�}�b�v��ݒ�
			SetUseShadowMap(0, _handleShadowMap);
		}

		if (!_objServer->Renderer()) { return false; }
	}

	// �`��Ɏg�p����V���h�E�}�b�v�̐ݒ������
	SetUseShadowMap(0, -1);

	DrawFormatString(0, 16, GetColor(255, 0, 0), "stage%s", _stageNum);

	_debug->Render();

	return true;
}

bool ModeGame::LoadData() {
	nlohmann::json j;
	std::ifstream file("res/map/stage" + _stageNum + ".json");

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

			if (iter->first.find(name) == 0) {
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

	//Spawner
	NEW TraserSpawner(_objServer);

	return true;
}

XGamePad* ModeGame::GetPad()const { return ApplicationMain::GetInstance()->GetPad(); }

