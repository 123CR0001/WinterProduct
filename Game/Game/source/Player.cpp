#include"Player.h"
#include"ObjectServer.h"
#include"ModeGame.h"
#include"appframe.h"
#include<vector>
#include<algorithm>
#include"Knife.h"
#include"ApplicationGlobal.h"
#include"ModeEffekseer.h"
#include"CapsuleComponent.h"
#include"AnimationConmponent.h"
#include"FollowCamera.h"
#include"OrbitCamera.h"
#include"MoveComponent.h"
#include"PhysWorld.h"
#include"SoundComponent.h"
#include"AfterImage.h"
#include"MotionComponent.h"

#include<functional>

Player::Player(ObjectServer* server)
	:CharaBase(server,"player")
	,_cameraCom(NEW FollowCamera(this,999))
	,_moveCom(NEW MoveComponent(this,1))
	,_motCom(NEW MotionComponent(this,200))
	,_weapon (NEW Knife(this))
	,_actionState(ACTION_STATE::kIdle)
	,_moveSpeedMag(1.f)
{
	server->SetPlayer(this);


	//このクラス特有のモーションデータのコマンド処理
	auto func = [this](const MOTION_DATA_ITEM& item) {_weapon->OnAttack(); _motCom->IncrementMotionCount(); };
	auto func2 = [this](const MOTION_DATA_ITEM& item) {_weapon->OffAttack(); _motCom->IncrementMotionCount();  };
	auto func3 = [this](const MOTION_DATA_ITEM& item) {ChangeState(item.ChangeMotion); _motCom->ResetMotionCount(); };

	//上記の処理をMotionComponentに追加
	_motCom->RegisterCustomCommand("ATTACK_ON",func);
	_motCom->RegisterCustomCommand("ATTACK_OFF", func2);
	_motCom->RegisterCustomCommand("CHANGE_MOTION", func3);

}

Player::~Player() {
	Player::Terminate();
}

bool Player::Initialize() {
	// モデルデータのロード（テクスチャも読み込まれる）
	CharaBase::Initialize();

	_cameraCom->Initialize();

	LoadModel("res/Chara/mannequin.mv1");

	//アニメーションの登録
	_anim->LoadAnimation("Idle", "mo_standby_01", 0);
	_anim->LoadAnimation("run", "mo_move_01", 0);
	_anim->LoadAnimation("Attack", "mo_lightsout_01", 1);
	_anim->LoadAnimation("Attack2", "mo_lightsout_01", 1);

	_capsule->SetMember(40.f, 30.f);

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
	return true;
}

bool Player::Process() {

	ACTION_STATE oldState = _actionState;

	auto pad = GetObjectServer()->GetGame()->GetPad();

	const float moveSpeed = 5.f * _moveSpeedMag;

	//入力処理
	switch (_actionState) {
	case ACTION_STATE::kIdle:
	case ACTION_STATE::kWalk:
		_actionState = ACTION_STATE::kIdle;

		if (pad->IsInputLeftStick()) {
			Vector3D vec(_pos - _cameraCom->GetPos());
			vec.Normalized();
			_eulerAngle.y = atan2f(vec.x, vec.z);

			float angle = atan2f((float)pad->GetLeftStick().x, (float)pad->GetLeftStick().y);

			_moveCom->SetRotateSpeed(angle);
			_moveCom->SetMoveSpeed(moveSpeed);

			_actionState = ACTION_STATE::kWalk;

		}

		if (pad->GetTrgButton() & INPUT_A && ModeServer::GetInstance()->IsAdd("LightsOut")) {
			_actionState = ACTION_STATE::kAttack;
		}
		if (pad->GetTrgButton() & INPUT_B) {
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
			Vector3D vec(_pos - _cameraCom->GetPos());
			vec.Normalized();
			_eulerAngle.y = atan2f(vec.x, vec.z);

			float angle = atan2f((float)pad->GetLeftStick().x, (float)pad->GetLeftStick().y);

			_moveCom->SetRotateSpeed(angle);
			_moveCom->SetMoveSpeed(moveSpeed / 2.f);

			_actionState = ACTION_STATE::kSilentWalk;

		}

		if (pad->GetTrgButton() & INPUT_B) {
			_actionState = ACTION_STATE::kIdle;
		}
		break;
	}

	ObjectBase::Process();

	// ステータスが変わっていないか？
	if(oldState != _actionState) {
		// ステータスに合わせてアニメーションのアタッチ
		switch(_actionState) {
		case ACTION_STATE::kIdle:
			_anim->ChangeAnimation("Idle");
			break;
		case ACTION_STATE::kWalk:
			_anim->ChangeAnimation("run");
			break;
		case ACTION_STATE::kAttack:
			_anim->ChangeAnimation("Attack");
			break;
		case ACTION_STATE::kAttack2:
			_anim->ChangeAnimation("Attack2");
			break;
		case ACTION_STATE::kSilent:
			_anim->ChangeAnimation("Idle");
			break;
		case ACTION_STATE::kSilentWalk:
			_anim->ChangeAnimation("run");
			break;
		}
	}

	//足音
	if (_moveCom->GetSpeed() >= moveSpeed) {
		new SoundComponent(this, 200.f);
	}


	//オブジェクトとの押出処理
	FixPos();

	return true;
}

bool Player::Render() {

	CharaBase::Render();

	int y = 0;
	//DrawFormatString(
	//	0, y, GetColor(255, 0, 0),
	//	"x %f y %f z %f",
	//	_pos.x,
	//	_pos.y,
	//	_pos.z,
	//	TRUE
	//); y += 25;
	//DrawFormatString(
	//	0, y, GetColor(255, 0, 0),
	//	"y　マトリックス %f",
	//	-atan2(_matrix.m[2][0], -_matrix.m[2][2]),
	//	TRUE
	//); y += 25;
	//DrawFormatString(
	//	0, y, GetColor(255, 0, 0),
	//	"y　オイラー %f",
	//	_eulerAngle.y,
	//	TRUE
	//); y += 25;
	////if (_is_stand) {
	////	DrawFormatString(0, 25, GetColor(255, 0, 0), "stand");
	////}

	//VECTOR pos = ConvWorldPosToViewPos(DxConverter::VecToDx(_pos));

	//DrawFormatString(0,y, GetColor(255, 0, 0), "再生時間\t%f", _anim->GetPlayTime()); y += 25;
	//DrawFormatString(0,y, GetColor(255, 0, 0), "総再生時間\t%f", _totalTime); y += 25;

	//DrawFormatString(
	//	0, y, GetColor(255, 0, 0),
	//	"marix X %f marix Y %f marix Z %f ",
	//	_matrix.m[3][0], _matrix.m[3][1], _matrix.m[3][2],
	//	TRUE
	//); y += 25;

	//MATRIX inverse = MInverse(_matrix);

	//MMult(inverse, _matrix);


	switch (_actionState) {
	case ACTION_STATE::kIdle:
		DrawFormatString(0, y, GetColor(255, 0, 0), "Idle"); y += 25;
		break;
	case ACTION_STATE::kWalk:
		DrawFormatString(0, y, GetColor(255, 0, 0), "Walk"); y += 25;
		break;
	case ACTION_STATE::kAttack:
		DrawFormatString(0, y, GetColor(255, 0, 0), "Attack"); y += 25;
		break;
	case ACTION_STATE::kAttack2:
		DrawFormatString(0, y, GetColor(255, 0, 0), "Attack2"); y += 25;
		break;
	case ACTION_STATE::kSilent:
		DrawFormatString(0, y, GetColor(255, 0, 0), "Silent"); y += 25;
		break;
	case ACTION_STATE::kSilentWalk:
		DrawFormatString(0, y, GetColor(255, 0, 0), "SilentWalk"); y += 25;
		break;

	}

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