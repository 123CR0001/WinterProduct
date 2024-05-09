#include"LightsOutComponent.h"

#include"SpriteTextFlipAnimation.h"

#include"UIHrsMinSec.h"
#include"UI.h"
#include"UISpriteText.h"
#include"UIScreen.h"
#include"UISecMiliSec.h"

#include"ModeGame.h"
#include"ModeColorIn.h"
#include"ModeColorOut.h"

#include"TimeLine.h"
#include"ObjectServer.h"
#include"Player.h"
#include"CommonSoldier.h"

#include"ApplicationMain.h"
#include"ApplicationGlobal.h"

#include"CameraZoomComponent.h"
#include"CreateAfterImageComponent.h"
#include"AnimationComponent.h"
#include"CameraComponent.h"

LightsOutComponent::LightsOutComponent(Player* owner, int order)
	:Component(owner, order)
	, _game(owner->GetObjectServer()->GetGame())
	,_player(owner)
	, _timerBg(NEW SpriteTextFlipAnimation(3, false))
	, _isUseLightsOut(NEW SpriteTextFlipAnimation(3, true))
	, _timer(NEW UISecMiliSec(Transform2(Vector2(359.f * SCREEN_WIDTH_MAG, 978.f * SCREEN_HEIGHT_MAG)), 100))
	, _noise(NEW SpriteTextFlipAnimation(8, true))
	, _hud(NEW SpriteText())
	, _afterImageCom(nullptr)
	, _limitCnt(600)
	, _useTimes(1)
	, _state(STATE::kNone)
	, _oldEnergyCount(0)
{
	//�^�C�}�[�̔w�i�̐ݒ�
	_timerBg->Stop();
	_timerBg->LoadDivText("res/UI/Game/Timer/ui_timerbg_01.png", 5, 1, 5, 500, 150);
	_timerBg->SetPos(Vector2(359.f * SCREEN_WIDTH_MAG, 978.f * SCREEN_HEIGHT_MAG));
	_timerBg->SetSize(Vector2(500.f * SCREEN_WIDTH_MAG, 150.f * SCREEN_HEIGHT_MAG));

	//�m�C�Y�̐ݒ�
	_noise->LoadDivText("res/Effect/ui_nightscope_1.png", 6, 1, 6, 1920, 1080);
	_noise->SetSize(Vector2(SCREEN_WIDTH, SCREEN_HEIGHT));
	_noise->SetPos(Vector2(SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f));
	_noise->SetAlpha(0.f);

	//HUD�̐ݒ�
	_hud->SetHandle(ResourceServer::LoadGraph("res/UI/Game/hud_01.png"));
	_hud->SetSize(Vector2(SCREEN_WIDTH, SCREEN_HEIGHT));
	_hud->SetPos(Vector2(SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f));
	_hud->SetAlpha(0.f);

	//���C�c�A�E�g�̎g�p�񐔂�\������UI�̐ݒ�
	_isUseLightsOut->LoadDivText("res/UI/Game/ui_timerbg_02.png", 5, 1, 5, 200, 200);
	_isUseLightsOut->SetSize(Vector2(200.f * SCREEN_WIDTH_MAG, 200.f * SCREEN_HEIGHT_MAG));
	_isUseLightsOut->SetPos(Vector2(523.f * SCREEN_WIDTH_MAG, 978.f * SCREEN_HEIGHT_MAG));
	_isUseLightsOut->SetAlpha(1.f);

	//�`�悵�Ȃ�
	_timer->SetIsDraw(false);

	//UI��ǉ�
	_game->GetUIServer()->AddUI(NEW UISpriteText(_noise, 300));
	_game->GetUIServer()->AddUI(NEW UISpriteText(_hud, 310));
	_game->GetUIServer()->AddUI(NEW UISpriteText(_timerBg, 200));
	_game->GetUIServer()->AddUI(NEW UISpriteText(_isUseLightsOut, 250));
	_game->GetUIServer()->AddUI(_timer);
}

LightsOutComponent::~LightsOutComponent()
{
}

bool LightsOutComponent::Process() {
	switch(_state) {
	case STATE::kNone:
		break;
	case STATE::kStart: {

		//�^�C�}�[�̔w�i�̃A�j���[�V�������n�߂�
		_timerBg->Play();

		//�c��̎g�p�񐔂����炷
		_useTimes--;

		//�^�C�}�[��30�t���[����ɕ`�悳���悤�ɂ���
		auto func = [this]() {
			_timer->SetIsDraw(true);
			};
		_game->GetTimeLine()->AddLine(30, func);

		//�t�F�[�h�C���A�E�g�𗘗p�������o
		ModeServer::GetInstance()->Add(NEW ModeColorOut(NEW ModeColorIn(10, true), nullptr, 10), 100, "Out");

		//���C�c�A�E�g���͕`�悷��
		_noise->SetAlpha(1.f);
		_hud->SetAlpha(1.f);

		//�v���C���[����c�����o�͂���悤�ɂ���
		_afterImageCom = NEW CreateAfterImageComponent(_player->GetAnimationComponent());

		//Zoom�C��
		NEW CameraZoomComponent(_player->GetCamera(), 0.6f, 60);

		//BGM�̍Đ�
		gGlobal._sndServer.Play("BGM_06");

		//�G���̗֊s����`��
		for(auto&& soldier : _game->GetObjectServer()->GetCommonSoldiers()) {
			for(int a = 0; a < MV1GetMaterialNum(soldier->GetHandle()); a++) {
				MV1SetMaterialOutLineWidth(soldier->GetHandle(), a, 0.5f);
			}
		}

		//���C�c�A�E�g���g�p���Ă��邱�Ƃ��Q�[���N���X�ɓ`����
		_game->SetIsUseLightsOut(true);

		//��Ԃ�Process�ɕύX
		_state = STATE::kProcess;

		break;
	}

	case STATE::kProcess: {

		//�c�莞�Ԃ����炷
		_limitCnt--;
		_timer->SetFrameCount(_limitCnt);

		//�c�莞�Ԃ�0�����ɂȂ�����A�I�������֓���
		if(_limitCnt < 0) {
			_state = STATE::kEnd;
		}

		if(_game->GetEnemyCount() == 0) {
			_state = STATE::kEnd;
		}

		//��b�o������ASE�𗬂�
		if(_limitCnt % 60 == 0) {
			//SE
			gGlobal._sndServer.Get("SE_15")->Play();
		}

		break;
	}
	case STATE::kEnd: {

		//�A�j���[�V�������t�Đ�������
		_timerBg->Reverse();

		//��\��
		_timer->SetIsDraw(false);

		//�t�F�[�h�C���A�E�g�𗘗p�������o
		ModeServer::GetInstance()->Add(NEW ModeColorOut(NEW ModeColorIn(10, true), nullptr, 10), 100, "Out");

		//�G�l�~�[���c���Ă���A�Q�[���I�[�o�[�̉��o������
		if(_game->GetEnemyCount() > 0) {
			SpriteTextFlipAnimation* text = NEW SpriteTextFlipAnimation(10, true);
			text->LoadDivText("res/UI/GameOver/ui_sirenfilter_01.png", 6, 1, 6, 1920, 1080);
			text->SetSize(Vector2(SCREEN_WIDTH, SCREEN_HEIGHT));
			text->SetPos(Vector2(SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f));
			_game->GetUIServer()->AddUI(NEW UISpriteText(text));
			gGlobal._sndServer.Play("SE_07");
		}

		//�c��g�p�񐔂�0��������
		if(_useTimes == 0) {
			//120�t���[����ɃQ�[���I�[�o�[���Q�[���N���A�����߂�
			auto func = [this]() {
				_game->SwitchOverOrClear();
				};

			_game->GetTimeLine()->AddLine(120, func);
		}

		//���C�c�A�E�g���͕`�悵�Ȃ�
		_noise->SetAlpha(0.f);
		_hud->SetAlpha(0.f);

		//Zoom�A�E�g
		NEW CameraZoomComponent(_player->GetCamera(), 1.f, 60);

		//BGM�̍Đ�
		gGlobal._sndServer.Play("BGM_03");

		//�G���̗֊s����`�悵�Ȃ�
		for(auto&& soldier : _game->GetObjectServer()->GetCommonSoldiers()) {
			for(int a = 0; a < MV1GetMaterialNum(soldier->GetHandle()); a++) {
				MV1SetMaterialOutLineWidth(soldier->GetHandle(), a, 0.f);
			}
		}

		//�v���C���[����폜
		if(_afterImageCom) {
			_afterImageCom->GetOwner()->DeleteComponent(_afterImageCom);
			_afterImageCom = nullptr;
		}

		//���C�c�A�E�g���g�p���Ă��Ȃ����Ƃ��Q�[���N���X�ɓ`����
		_game->SetIsUseLightsOut(false);

		_state = STATE::kNone;
		break;
	}

	default: {
		//�G���[����
	}
	}

	//���C�c�A�E�g���g�p�\�ɂȂ�����AUI��\���B�����āASE�𗬂�
	if(_useTimes > 0 && _game->GetEnergyCount() == 0 && _oldEnergyCount != _game->GetEnergyCount()) {
		_isUseLightsOut->SetAlpha(1.f);
		gGlobal._sndServer.Play("SE_09");
	}

	//UI���\��
	//_oldEnergyCount���X�V
	else {
		_isUseLightsOut->SetAlpha(0.f);
		_oldEnergyCount = _game->GetEnergyCount();
	}

	return true;
}