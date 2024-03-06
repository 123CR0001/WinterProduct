
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

#include"CameraZoomComponent.h"
#include"CameraComponent.h"

ModeGame::ModeGame(std::string stageNum) 
	:_objServer(NEW ObjectServer(this))
	,_debug(NEW ModeDebugMenu(this))
	,_modeEffekseer(NEW ModeEffekseer())
	,_stage(stageNum)
	,_energyCount(0)
{
	_uiServer = NEW UIServer();
	_fps = NEW FPS();


	ModeServer::GetInstance()->Add(_modeEffekseer, 100, MODE_EFFEKSEER_NAME);
	_debug->Initialize();
}

ModeGame::~ModeGame() {
	delete _objServer;
	delete _debug;
	delete _uiServer;
	delete _fps;
}

bool ModeGame::Initialize() {
	if (!base::Initialize()) { return false; }


	// �A�v���P�[�V�����O���[�o���̏�����
	gGlobal.Init();


	_objServer->LoadData(_stage);

	ModeServer::GetInstance()->Add(new ModeMiniMap(this), 100, "MiniMap");

	_isCouldLightsOut = false;

	// �V���h�E�}�b�v�̐���
	_handleShadowMap = MakeShadowMap(2048, 2048);


	_uiServer->Add(NEW UILightsTimer(), nullptr, 0, 0, 930, 600, 150, 100, "lightsOutTimer");
	// �ŏ��̓X���C�h������Ȃ��̂ŁA�����ʒu�ƏI���ʒu�𓯂��ɂ��Ă���
	_uiServer->Add(NEW UIRemainingUses(this, 2, 930, 2, 930,3), nullptr, 0, 2, 930, 450, 150, 100, "remainingUses");
	_cntTest = 0;

	_bg = ResourceServer::LoadGraph("res/gamemain_bg.png");

	return true;
}

bool ModeGame::Terminate() {

	base::Terminate();

	DeleteShadowMap(_handleShadowMap);
	_objServer->Terminate();
	_uiServer->Terminate();
	ModeServer::GetInstance()->Del(ModeServer::GetInstance()->Get("MiniMap"));

	return true;
}

bool ModeGame::Process() {
	base::Process();
	// fps�̍X�V
	_fps->Update();	
	
	_uiServer->Process();

	if (_debug->Process()) { return true; }

	//�I�u�W�F�N�g�T�[�o�[�̏���
	if (!_objServer->ProcessInit()) { return false; }
	if (!_objServer->Process()) { return false; }

	//LightsOut���[�h��ǉ�
	if (
		GetPad()->GetTrgButton() & INPUT_Y  
		&& !ModeServer::GetInstance()->IsAdd("Out")
		&& !ModeServer::GetInstance()->IsAdd("LightsOut") 
		&& _energyCount == 0
		) {

		gGlobal._sndServer.Get("SE_09")->Play();

		auto func = [this]() {
			ModeServer::GetInstance()->Add(NEW ModeLightsOut(this), 100, "LightsOut"); 
			//�v���C���[����c�����o�͂���悤�ɂ���
			NEW CreateAfterImageComponent(_objServer->GetPlayer()->GetAnimationComponent());
			NEW CameraZoomComponent(_objServer->GetPlayer()->GetCamera(), -0.3f, 60);


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
			ModeServer::GetInstance()->Add(NEW ModeGameOver(this), 100, "GameOver");
			ModeServer::GetInstance()->Del(this);
		}
		else {
			ModeServer::GetInstance()->Add(NEW ModeClear(this), 100, "GameClear");
		}
	}

	if (GetPad()->GetTrgButton() & INPUT_DPAD_UP) {
		_cntTest++;
	}

	//if (GetPad()->GetTrgButton() & INPUT_X) {
	//	_uiServer->Search("remainingUses")->Selected();
	//	_uiServer->Search("lightsOutTimer")->Selected();
	//}

	if(GetPad()->GetTrgButton() & INPUT_LEFT_THUMB) {
		auto func = [this]() {
			// ���[�h�̍폜
			ModeServer::GetInstance()->Del(ModeServer::GetInstance()->Get("ui"));
			ModeServer::GetInstance()->Del(ModeServer::GetInstance()->Get(MODE_EFFEKSEER_NAME));
			ModeServer::GetInstance()->Del(this);
			// ���̃��[�h��o�^
			ModeServer::GetInstance()->Add(NEW ModeClear(this), 1, "GameClear");
			};
		// ���̃��[�h��o�^
		ModeColorIn* colorIn = new ModeColorIn(60, true);
		ModeBase* mode = new ModeColorOut(colorIn, func);
		ModeServer::GetInstance()->Add(mode, 11, "Out");
	}

	// fps�̑ҋ@
	//_fps->WaitFPS();
	//auto CommonSoldiers = GetObjectServer()->GetCommonSoldiers();
	//if (CommonSoldiers.size() == 0) { _fps->SetFPS(50); }

	return true;
}

bool ModeGame::Render() {
	base::Render();
	// 3D��{�ݒ�
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

	// ���C�g�ݒ�
	SetUseLighting(TRUE);

	VECTOR vLightDir = VGet(-1, -1, 0.5f);
	VECTOR target = _objServer->GetPlayer()->GetDxPos();
#if 0	// ���s���C�g
	SetGlobalAmbientLight(GetColorF(0.f, 0.f, 0.f, 0.f));
	ChangeLightTypeDir(vLightDir);
#endif
#if 1	// �|�C���g���C�g
	SetGlobalAmbientLight(GetColorF(0.f, 0.f, 0.f, 0.f));
	ChangeLightTypePoint(VAdd(target, VGet(0, 50.f, 0)), 1000.f, 0.f, 0.005f, 0.f);
#endif

	// �V���h�E�}�b�v���z�肷�郉�C�g�̕������Z�b�g
	SetShadowMapLightDirection(_handleShadowMap, vLightDir);

	// �V���h�E�}�b�v�ɕ`�悷��͈͂�ݒ�
	target.y += 50.f;

	SetShadowMapDrawArea(_handleShadowMap,VAdd(target, VGet(-800.0f, -1.0f, -800.0f)), VAdd(target, VGet(800.0f, 800.0f, 800.0f)));

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

	_uiServer->Render();

	_debug->Render();

	return true;
}

XGamePad* ModeGame::GetPad()const { return ApplicationMain::GetInstance()->GetPad(); }

