
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
#include"UIVision.h"
#include"UIMiniMap.h"
#include"SpriteNumber.h"
#include"UISpriteText.h"
#include"SoundItem.h"

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
	,_enemyCountText(NEW SpriteNumber(_enemyCount,2))
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

	// �V���h�E�}�b�v�̐���
	_handleShadowMap = MakeShadowMap(2048, 2048);

	//�w�i
	_bg = ResourceServer::LoadGraph("res/gamemain_bg.png");

	//���C�c�A�E�g
	_lightsOut = NEW LightsOut(this);

	//�c��̓G�̐���`��Ŏg�p����摜�̐ݒ�
	_enemyCountText->LoadDivNumber("res/UI/Result/ui_timer_01.png", 5, 2, 46, 70);
	_enemyCountText->SetPos(Vector2(365.f * SCREEN_WIDTH_MAG,858.f*SCREEN_HEIGHT_MAG));
	_enemyCountText->SetSize(Vector2(46.f * SCREEN_WIDTH_MAG, 70.f * SCREEN_HEIGHT_MAG));
	_enemyCountText->SetAlpha(0.f);

	//��L�̔w�i
	{
		_enemyCountBg = NEW SpriteText(
			ResourceServer::LoadGraph("res/UI/Game/CommonSoldierTimes.png"),
			Transform2(Vector2(224.f * SCREEN_WIDTH_MAG, 847.f * SCREEN_HEIGHT_MAG)),
			Vector2(160.f * SCREEN_WIDTH_MAG, 120.f * SCREEN_HEIGHT_MAG)
		);
		_enemyCountBg->SetAlpha(0.f);
		_uiServer->AddUI(NEW UISpriteText(_enemyCountBg,100));
	}
	_uiServer->AddUI(NEW UISpriteText(_enemyCountText, 50));

	_uiServer->AddUI(NEW UIDetectionLevel(_objServer,100));
	_uiServer->AddUI(NEW UIVision(_objServer,1000));
	_uiServer->AddUI(NEW UIMiniMap(this,100));


	gGlobal._sndServer.Play("BGM_01");
	return true;
}

bool ModeGame::Terminate() {

	base::Terminate();

	DeleteShadowMap(_handleShadowMap);
	_objServer->Terminate();

	//BGM���~�߂�
	gGlobal._sndServer.StopType(SoundItemBase::TYPE::BGM);

	return true;
}

bool ModeGame::Process() {
	base::Process();

	_timeLine->Process();
	_uiServer->Process();
	_lightsOut->Process();

	// fps�̍X�V
	_fps->Update();	
	
	if (_debug->Process()) { return true; }
	if (!_objServer->ProcessInit()) { return false; }
	if (!_objServer->Process()) { return false; }

	//LightsOut���[�h��ǉ�
	if (_lightsOut->IsUse()
		&& GetPad()->GetTrgButton() & INPUT_Y  
		&& _energyCount == 0
		) {
		_lightsOut->Use();
		_enemyCountBg->SetAlpha(1.f);
		_enemyCountText->SetAlpha(1.f);
	}

	//Pause���[�h��ǉ�
	if (GetPad()->GetTrgButton() & INPUT_START && !ModeServer::GetInstance()->IsAdd("Pause")) {
		ModeServer::GetInstance()->Add(NEW ModePause(), 100, "Pause");
	}

	//�`�悷�鐔���̍X�V
	_enemyCountText->SetNumber(_enemyCount);

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

	_uiServer->Draw();
	_debug->Render();

	return true;
}

XGamePad* ModeGame::GetPad()const { return ApplicationMain::GetInstance()->GetPad(); }

void ModeGame::SwitchOverOrClear() {
	if(_enemyCount == 0) {
		{
			
			_objServer->GetPlayer()->ChangeState("Clear");
			NEW CameraZoomComponent(_objServer->GetPlayer()->GetCamera(),0.5f,60);
		}
		//�N���A��ʂ̑J��
		{
			auto timeLineFunc = [=]()mutable {
				auto func = [this]() {
					// ���[�h�̍폜	
					ModeServer::GetInstance()->Del(ModeServer::GetInstance()->Get(MODE_EFFEKSEER_NAME));
					ModeServer::GetInstance()->Del(this);
					// ���̃��[�h��o�^
					ModeServer::GetInstance()->Add(NEW ModeClear(_resultData), 100, "GameClear");
					};
				// ���̃��[�h��o�^
				ModeColorIn* colorIn = NEW ModeColorIn(60, true);
				ModeBase* mode = NEW ModeColorOut(colorIn, func);
				ModeServer::GetInstance()->Add(mode, 100, "Out");
				};
			_timeLine->AddLine(180, timeLineFunc); 
		}
	}
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
				// ���̃��[�h��o�^
				ModeColorIn* colorIn = NEW ModeColorIn(60, true);
				ModeBase* mode = NEW ModeColorOut(colorIn, func);
				ModeServer::GetInstance()->Add(mode, 100, "Out");
				};
			_timeLine->AddLine(180, timeLineFunc); 
		}
	}
}

void ModeGame::DecrementEnemyCount() {
	_enemyCount--;

	if(_enemyCount == 0) {
		auto func = [=]()mutable { SwitchOverOrClear(); };
		_timeLine->AddLine(60, func);

		_lightsOut->Stop();
	}
}