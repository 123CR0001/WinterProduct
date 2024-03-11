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

#include<functional>

Player::Player(ObjectServer* server)
	:CharaBase(server,"player")
	,_cameraCom(NEW FollowCamera(this,999))
	,_anim(NEW PlayerAnimationComponent(this,1000))
	,_weapon (NEW Knife(this))
	,_actionState(ACTION_STATE::kIdle)
	,_capsule(NEW CapsuleComponent(this,1000))
	,_moveSpeedMag(1.f)
	,_isLightsOut(false)
{
	server->SetPlayer(this);

	_motCom = NEW MotionComponent(_anim,1001);

	//このクラス特有のモーションデータのコマンド処理
	auto func = [this](const MOTION_DATA_ITEM& item) {_weapon->OnAttack(); _motCom->IncrementMotionCount(); };
	auto func2 = [this](const MOTION_DATA_ITEM& item) {_weapon->OffAttack(); _motCom->IncrementMotionCount();  };
	auto func3 = [this](const MOTION_DATA_ITEM& item) {ChangeState(item.ChangeMotion); _motCom->ResetMotionCount(); };

	//上記の処理をMotionComponentに追加
	_motCom->RegisterCustomCommand("ATTACK_ON",func);
	_motCom->RegisterCustomCommand("ATTACK_OFF", func2);
	_motCom->RegisterCustomCommand("CHANGE_MOTION", func3);

	NEW CountClearTimeComponent(this);

}

Player::~Player() {
}

bool Player::Initialize() {
	// モデルデータのロード（テクスチャも読み込まれる）
	CharaBase::Initialize();

	_cameraCom->Initialize();

	_capsule->SetMember(40.f, 30.f);
	_capsule->AddSkipName("Decoy");

	_actionState = ACTION_STATE::kIdle;
	_anim->ChangeAnimation("Idle");

	//カメラと同じ方向を向く
	Vector3D vec(_pos - _cameraCom->GetPos());
	vec.Normalized();
	_eulerAngle.y = atan2f(vec.x, vec.z);

	ModelMatrixSetUp();
	return true;
}

bool Player::Terminate() {
	CharaBase::Terminate();
	GetObjectServer()->SetPlayer(nullptr);
	return true;
}

bool Player::Process() {

	auto pad = GetObjectServer()->GetGame()->GetPad();
	auto trg = pad->GetTrgButton();

	const float moveSpeed = 5.f * _moveSpeedMag;

	//入力処理
	switch (_actionState) {
	case ACTION_STATE::kIdle:
	case ACTION_STATE::kWalk:
		_actionState = ACTION_STATE::kIdle;

		if (pad->IsInputLeftStick()) {

			_eulerAngle.y = -_cameraCom->GetAngle().y;

			float angle = atan2f((float)pad->GetLeftStick().x, (float)pad->GetLeftStick().y);

			_moveCom->SetRotateSpeed(angle);
			_moveCom->SetMoveSpeed(moveSpeed);

			_actionState = ACTION_STATE::kWalk;

		}

		if (trg & INPUT_X && _isLightsOut) {
			_actionState = ACTION_STATE::kAttack;

			gGlobal._sndServer.Get("SE_02")->Play();
		}
		if (trg & INPUT_X && !_isLightsOut) {
			NEW Decoy(this);
		}
		if (trg & INPUT_A) {
			_actionState = ACTION_STATE::kSilent;
		}
		break;
	case ACTION_STATE::kAttack:
		break;
	case ACTION_STATE::kAttack2:
		break;
	case ACTION_STATE::kSilent:
	case ACTION_STATE::kSilentWalk:
		_actionState = ACTION_STATE::kSilent;

		if (pad->IsInputLeftStick()) {

			_eulerAngle.y = -_cameraCom->GetAngle().y;

			float angle = atan2f((float)pad->GetLeftStick().x, (float)pad->GetLeftStick().y);

			_moveCom->SetRotateSpeed(angle);
			_moveCom->SetMoveSpeed(moveSpeed / 2.f);

			_actionState = ACTION_STATE::kSilentWalk;

		}

		if (trg & INPUT_A) {
			_actionState = ACTION_STATE::kIdle;
		}
		break;
	}

	ObjectBase::Process();

	//足音
	if (_moveCom->GetSpeed() >= moveSpeed && !ModeServer::GetInstance()->IsAdd("LightsOut")) {
		new SoundComponent(this, 200.f);
	}	


	return true;
}

bool Player::Render() {

	CharaBase::Render();

	const char* name = nullptr;

	switch (_actionState) {
	case ACTION_STATE::kIdle:
		name = "idle";
		break;
	case ACTION_STATE::kWalk:
		name = "kWalk";
		break;
	case ACTION_STATE::kAttack:
		break;
	case ACTION_STATE::kAttack2:
		break;
	case ACTION_STATE::kSilent:
		name = "kSilent";
		break;
	case ACTION_STATE::kSilentWalk:
		name = "kSilentWalk";
		break;
	}
	DrawFormatString(0, 0, GetColor(255, 0, 0), "%d", _motCom->GetMotionCount());
	return true;
}

bool Player::ChangeState(std::string stateName) {
	std::unordered_map<std::string, ACTION_STATE>convert{
		{"Idle", Player::ACTION_STATE::kIdle},
		{ "Walk",Player::ACTION_STATE::kWalk },
		{ "Attack",Player::ACTION_STATE::kAttack },
		{ "Attack2",Player::ACTION_STATE::kAttack2 },
	};

	if (convert.find(stateName) != convert.end()) {
		_actionState = convert[stateName];
		return true;
	}

	return false;
}