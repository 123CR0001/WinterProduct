
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

	//�X�e�[�W�̓ǂݍ���
	_objServer->LoadData(_stage);

	// �V���h�E�}�b�v�̐���
	_handleShadowMap = MakeShadowMap(2048, 2048);

	//�w�i
	_bg = ResourceServer::LoadGraph("res/gamemain_bg.png");

	//�c��̓G�̐���`��Ŏg�p����摜�̐ݒ�
	_enemyCountText->LoadDivNumber("res/UI/Result/ui_timer_01.png", 5, 2, 46, 70);
	_enemyCountText->SetPos(Vector2(365.f * SCREEN_WIDTH_MAG,858.f*SCREEN_HEIGHT_MAG));
	_enemyCountText->SetSize(Vector2(46.f * SCREEN_WIDTH_MAG, 70.f * SCREEN_HEIGHT_MAG));
	_enemyCountText->SetAlpha(0.f);

	//_enemyCountText�̔w�i
	{
		_enemyCountBg = NEW SpriteText(
			ResourceServer::LoadGraph("res/UI/Game/CommonSoldierTimes.png"),
			Transform2(Vector2(224.f * SCREEN_WIDTH_MAG, 847.f * SCREEN_HEIGHT_MAG)),
			Vector2(160.f * SCREEN_WIDTH_MAG, 120.f * SCREEN_HEIGHT_MAG)
		);
		_enemyCountBg->SetAlpha(0.f);
		_UIScreen->AddUI(NEW UISpriteText(_enemyCountBg,100));
	}

	//UI�̒ǉ�
	_UIScreen->AddUI(NEW UISpriteText(_enemyCountText, 50));
	_UIScreen->AddUI(NEW UIDetectionLevel(_objServer,100));
	_UIScreen->AddUI(NEW UIVision(_objServer,1000));
	_UIScreen->AddUI(NEW UIMiniMap(this,100));


	gGlobal._sndServer.Play("BGM_03");
	return true;
}

bool ModeGame::Terminate() {

	base::Terminate();

	// �V���h�E�}�b�v�̍폜
	DeleteShadowMap(_handleShadowMap);

	//�I�u�W�F�N�g�T�[�o�[�̏I������
	_objServer->Terminate();

	//BGM���~�߂�
	gGlobal._sndServer.StopType(SoundItemBase::TYPE::BGM);

	return true;
}

bool ModeGame::Process() {
	base::Process();

	//�X�V
	_timeLine->Process();
	_UIScreen->Process();

	if (!_objServer->Process()) { return false; }

	//���C�c�A�E�g���g�p���Ȃ�A�ꕔ��UI�̕\������
	if (_isUsingLightsOut) {
		if(_enemyCountBg->GetAlpha() != 1.f)_enemyCountBg->SetAlpha(1.f);
		if(_enemyCountText->GetAlpha() != 1.f)_enemyCountText->SetAlpha(1.f);
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

	// �w�i�`��
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

	// �|�C���g���C�g
	SetGlobalAmbientLight(GetColorF(0.f, 0.f, 0.f, 0.f));
	ChangeLightTypePoint(VAdd(target, VGet(0, 50.f, 0)), 1000.f, 0.f, 0.005f, 0.f);

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

	//UI�̕`��
	_UIScreen->Draw();

	return true;
}

XGamePad* ModeGame::GetPad()const { return ApplicationMain::GetInstance()->GetPad(); }

void ModeGame::SwitchOverOrClear() {
	//�G�̐���0�Ȃ�N���A
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
	//�G�̐���0�łȂ��Ȃ�Q�[���I�[�o�[
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
	//�G�̐������炷
	_enemyCount--;

	//�G�̐���0�Ȃ�N���A�������s��
	if(_enemyCount == 0) {

		//60�t���[����ɃN���A�������s��
		_timeLine->AddLine(60, [=]()mutable { SwitchOverOrClear(); });
	}
}