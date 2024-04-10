#include"Player.h"
#include"ObjectServer.h"
#include"ModeGame.h"
#include"appframe.h"
#include<algorithm>
#include"Knife.h"
#include"ApplicationGlobal.h"
#include"ModeEffekseer.h"
#include"CapsuleComponent.h"
#include"PlayerAnimationComponent.h"
#include"FollowCamera.h"
#include"OrbitCamera.h"
#include"MoveComponent.h"
#include"PhysWorld.h"
#include"SoundComponent.h"
#include"AfterImage.h"
#include"MotionComponent.h"
#include"Decoy.h"

#include"CountClearTimeComponent.h"
#include"CameraZoomComponent.h"

#include<functional>

#include"ModeGameOver.h"
#include"TimeLine.h"

#include"ModeColorIn.h"
#include"ModeColorOut.h"

#include"SpriteNumber.h"
#include"UISpritetext.h"
#include"MyUIServer.h"

#include"ApplicationMain.h"

Player::Player(ObjectServer* server)
	:CharaBase(server,"player")
	,_cameraCom(NEW FollowCamera(this,999))
	,_anim(NEW PlayerAnimationComponent(this,10000))
	,_weapon (NEW Knife(this))
	,_actionState(ACTION_STATE::kIdle)
	,_capsule(NEW CapsuleComponent(this,1000))
	,_moveSpeedMag(1.f)
	,_isLightsOut(false)
	,_decoyTimes(1)
	,_decoyTimesText(NEW SpriteNumber(_decoyTimes))
{

	//���[�V�����f�[�^���Q�Ƃ��Ă��鏈��
	_motCom = NEW MotionComponent(_anim, 1000);
	
	//�I�u�W�F�N�g�T�[�o�[�Ƀv���C���[�N���X�o�^�p�|�C���^�ϐ��Ɏ��g�̃A�h���X��o�^
	server->SetPlayer(this);

	//���̃N���X���L�̃��[�V�����f�[�^�̃R�}���h����
	auto func = [this](const MOTION_DATA_ITEM& item) {_weapon->OnAttack(); _motCom->IncrementMotionCount(); };
	auto func2 = [this](const MOTION_DATA_ITEM& item) {_weapon->OffAttack(); _motCom->IncrementMotionCount();  };
	auto func3 = [this](const MOTION_DATA_ITEM& item) {ChangeState(item.ChangeMotion); };

	//��L�̏�����MotionComponent�ɒǉ�
	_motCom->RegisterCustomCommand("ATTACK_ON",func);
	_motCom->RegisterCustomCommand("ATTACK_OFF", func2);
	_motCom->RegisterCustomCommand("CHANGE_MOTION", func3);

	//���U���g�̃N���A�^�C�����W�v
	NEW CountClearTimeComponent(this);

	_weapon->SetFrameName("Owl_RightHand");
}

Player::~Player() {

}

bool Player::Initialize() {
	// ���f���f�[�^�̃��[�h�i�e�N�X�`�����ǂݍ��܂��j
	CharaBase::Initialize();

	//�J�����̏�����
	_cameraCom->Initialize();

	//�J�v�Z���̐ݒ�
	_capsule->SetMember(40.f, 30.f);
	_capsule->AddSkipName("Decoy");

	//��Ԃ��hkIdle�h�ɁA�A�j���[�V���������l
	_actionState = ACTION_STATE::kIdle;
	_anim->ChangeAnimation("Idle");

	//�J�����Ɠ�������������
	Vector3 vec(_pos - _cameraCom->GetPos());
	vec.Normalized();
	_eulerAngle.y = atan2f(vec.x, vec.z);

	//�f�R�C�̎c��g�p�񐔂�UI
	_decoyTimesText->LoadDivNumber("res/UI/Result/ui_timer_01.png", 5, 2, 46, 70);
	_decoyTimesText->SetPos(Vector2(337.f * SCREEN_WIDTH_MAG, 1004.5f * SCREEN_HEIGHT_MAG));
	_decoyTimesText->SetSize(Vector2(40.f * SCREEN_WIDTH_MAG, 76 * SCREEN_WIDTH_MAG));

	GetObjectServer()->GetGame()->GetUIServer()->AddUI(NEW UISpriteText(_decoyTimesText, 100));

	//��L�̔w�i
	_decoyTimesTextBg = NEW SpriteText(
	ResourceServer::LoadGraph("res/UI/Game/ui_itembg_01.png")
		, Transform2(Vector2(231.f * SCREEN_WIDTH_MAG, 978.f * SCREEN_HEIGHT_MAG))
		, Vector2(400.f * SCREEN_WIDTH_MAG, 150.f * SCREEN_WIDTH_MAG)
		);
	GetObjectServer()->GetGame()->GetUIServer()->AddUI(
		NEW UISpriteText(
			_decoyTimesTextBg
			, 1000
		)
	);

	//���f���ɍ��̐ݒ�𔽉f
	ModelMatrixSetUp();

	return true;
}

bool Player::Terminate() {
	CharaBase::Terminate();

	//�I�u�W�F�N�g�T�[�o�[�Ƀv���C���[�N���X�o�^�p�|�C���^�ϐ���nullptr�ɂ���
	GetObjectServer()->SetPlayer(nullptr);
	return true;
}

bool Player::Process() {

	//�Q�[���p�b�h�̓��͒l
	auto pad = GetObjectServer()->GetGame()->GetPad();

	//�g���K�[�l
	auto trg = pad->GetTrgButton();

	//�ړ��X�s�[�h
	const float moveSpeed = 5.f * _moveSpeedMag;

	//���͏���
	switch (_actionState) {
	case ACTION_STATE::kIdle:
	case ACTION_STATE::kWalk: {
		_actionState = ACTION_STATE::kIdle;

		float angle = 0.f;

		if(pad->IsInputLeftStick()) {
			angle = pad->GetLeftStickAtan2();

			//�v���C���[����J������Y���̊p�x�𔽓]
			_eulerAngle.y = -_cameraCom->GetAngle().y;

			//�ړ����x�A�p�x��o�^
			_moveCom->SetRotateSpeed(angle);
			_moveCom->SetMoveSpeed(moveSpeed);

			//��Ԃ��hkWalk�h��
			_actionState = ACTION_STATE::kWalk;

		}

		//�U��(�p�^�[��2)
		if(trg & INPUT_X && _isLightsOut) {
			_actionState = ACTION_STATE::kAttack2;

			gGlobal._sndServer.Get("SE_02")->Play();
		}
		//�U��(�p�^�[��3)
		if(trg & INPUT_A && _isLightsOut) {
			_actionState = ACTION_STATE::kAttack3;
			gGlobal._sndServer.Get("SE_02")->Play();
		}

		//�f�R�C�̎g�p
		if(trg & INPUT_X && !_isLightsOut && _decoyTimes > 0) {
			NEW Decoy(this, angle);
			_decoyTimes--;
		}

		//��Ԃ��hkSilent�h��
		if(trg & INPUT_A && !_isLightsOut) {
			_actionState = ACTION_STATE::kSilent;
		}
		break;
	}
	case ACTION_STATE::kSilent:
	case ACTION_STATE::kSilentWalk:
		_actionState = ACTION_STATE::kSilent;

		//�hkIdle�h��Ԃ̈ړ��Ə����͓���
		if (pad->IsInputLeftStick()) {

			_eulerAngle.y = -_cameraCom->GetAngle().y;

			float angle = atan2f((float)pad->GetLeftStick().x, (float)pad->GetLeftStick().y);

			_moveCom->SetRotateSpeed(angle);

			//�ړ����x������
			_moveCom->SetMoveSpeed(moveSpeed / 2.f);

			_actionState = ACTION_STATE::kSilentWalk;

		}

		//��Ԃ��hkIdle�h��
		if (trg & INPUT_A) {
			_actionState = ACTION_STATE::kIdle;
		}

		//���C�c�A�E�g�ɂȂ�����A��Ԃ�߂�
		if (_isLightsOut) {
			_actionState = ACTION_STATE::kIdle;
		}
		break;
	}

	ObjectBase::Process();
	//���S����
	//��x����
	if(_state == STATE::kDead && _actionState != ACTION_STATE::kDeath) {

		GetObjectServer()->GetGame()->GetTimeLine()->AddLine(240,
			[=]() mutable{
				NEW CameraZoomComponent(_cameraCom, 1.f, 60);

				auto func = [=]() {
					NEW CameraZoomComponent(_cameraCom, 1.f, 60);
					ModeServer::GetInstance()->Del(ModeServer::GetInstance()->Get(MODE_EFFEKSEER_NAME));
					ModeServer::GetInstance()->Del(ModeServer::GetInstance()->Get("game"));
					ModeServer::GetInstance()->Add(NEW ModeGameOver(GetObjectServer()->GetGame()), 100, "GameOver"); 
				};

				ModeServer::GetInstance()->Add(NEW ModeColorOut(NEW ModeColorIn(60, true), func, 60), 1000, "Out");
			}
		);

		_state = STATE::kNone;
		_actionState = ACTION_STATE::kDeath;
		return true;
	}

	//����
	if (_moveCom->GetSpeed() >= moveSpeed && !ModeServer::GetInstance()->IsAdd("LightsOut")) {
		new SoundComponent(this, 200.f);
	}	

	//�_���[�W���󂯂�
	if(_damageData.isDamage) {
		//��Ԃ��hkDead�h��
		_state = STATE::kDead;

		//��ɂ���
		_damageData = DamageData{};
	}

	//���C�c�A�E�g���g�p���Ă���Ȃ�A�f�R�C�̎g�p�񐔂�`�悷��UI���\����
	if(_isLightsOut) {
		_decoyTimesText->SetAlpha(0.f);
	}

	//UI�̍X�V
	_decoyTimesText->SetNumber(_decoyTimes);

	//3D�T�E���h�̏����X�V
	Set3DSoundListenerPosAndFrontPos_UpVecY(GetDxPos(), DxConverter::VecToDx(_eulerAngle));

	return true;
}

bool Player::Render() {

	CharaBase::Render();

	return true;
}

bool Player::ChangeState(std::string stateName) {
	std::unordered_map<std::string, ACTION_STATE>convert{
		{"Idle", Player::ACTION_STATE::kIdle},
		{ "Walk",Player::ACTION_STATE::kWalk },
		{ "Attack",Player::ACTION_STATE::kAttack },
		{ "Attack2",Player::ACTION_STATE::kAttack2 },
		{"Dead",Player::ACTION_STATE::kDeath},
		{"Clear",Player::ACTION_STATE::kClear}
	};

	if (convert.find(stateName) != convert.end()) {
		_actionState = convert[stateName];
		return true;
	}

	return false;
}