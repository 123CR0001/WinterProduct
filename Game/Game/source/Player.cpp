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

Player::Player(ObjectServer* server)
	:CharaBase(server) 
	,_cameraCom(new FollowCamera(this,999))
	,_moveCom(new MoveComponent(this,1))
	,_weapon ( new Knife(this))
{
	server->SetPlayer(this);
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
	_anim->LoadAnimation("attack", "mo_lightsout_01", 1);
	_anim->LoadAnimation("attack2", "mo_lightsout_01", 1);

	// 3Dモデルの1番目のアニメーションをアタッチする

	_colSubY = 40.f;
	_radian = 30;

	_capsule->SetMember(40.f, 30.f);

	_actionState = ACTION_STATE::kIdle;
	_anim->ChangeAnimation("Idle");

	ModelMatrixSetUp();
	return true;
}

bool Player::Terminate() {
	CharaBase::Terminate();
	return true;
}

bool Player::Process() {

	ACTION_STATE old_state = _actionState;

	auto pad = GetObjectServer()->GetGame()->GetPad();

	// 移動前の位置を保存
	_oldPos = _pos;

	const float moveSpeed = 5.f;

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

		if (pad->GetTrgButton() & INPUT_A) {
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
			_moveCom->SetMoveSpeed(moveSpeed/2.f);

			_actionState = ACTION_STATE::kSilentWalk;

		}

		if (pad->GetTrgButton() & INPUT_B) {
			_actionState = ACTION_STATE::kIdle;
		}
		break;
	}

	MotionProcess();

	// ステータスが変わっていないか？
	if(old_state != _actionState) {
		// ステータスに合わせてアニメーションのアタッチ
		switch(_actionState) {
		case ACTION_STATE::kIdle:
			_anim->ChangeAnimation("Idle");
			break;
		case ACTION_STATE::kWalk:
			_anim->ChangeAnimation("run");
			break;
		case ACTION_STATE::kAttack:
			_anim->ChangeAnimation("attack");
			break;
		case ACTION_STATE::kAttack2:
			_anim->ChangeAnimation("attack2");
			break;
		}
		_motCnt = 0;
	}


	ObjectBase::Process();

	//足音
	if (Vector3D::LengthSquare(_pos, _oldPos) >= moveSpeed * moveSpeed) {
		new SoundComponent(this, 500.f);
	}

	//オブジェクトとの押出処理
	FixPos();

	return true;
}

bool Player::Render() {

	//MV1SetAttachAnimTime(_handle, _attachIndex, _playTime);
	//MV1DrawModel(_handle);
	CharaBase::Render();

	Vector3D vec(_pos - _cameraCom->GetPos());

	vec.Normalized();

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

bool Player::MotionProcess() {
	auto motionData = gGlobal._charaMotionData;

	//モーションの再生時間とJsonデータで指定されているフレームが同じだっと時に処理を実行
	
	if (motionData[MotionType::PLAYER].find((unsigned int)_actionState)!= motionData[MotionType::PLAYER].end()){
		auto charaMotionData = motionData[MotionType::PLAYER][(unsigned int)_actionState][_motCnt];

		if ((int)(_anim->GetPlayTime() + 0.999) == charaMotionData.playTime) {

			//次のモーションデータを参照したいなら、_motCnt++をする

			switch (charaMotionData.Command) {
			case MotionCommand::ATTACK_ON:
			{
				_weapon->OnAttack();
				_motCnt++;
				break;
			}
			case MotionCommand::ATTACK_OFF:
			{
				_weapon->OffAttack();
				_motCnt++;
				break;
			}
			case MotionCommand::PLAY_EFFECT:
			{
				_motCnt++;
				break;
			}
			case MotionCommand::PLAY_EFFECT_3DFRAME:
			{

				//エフェクトの再生
				void* mode = ModeServer::GetInstance()->Get(MODE_EFFEKSEER_NAME);
				if (mode) {

					// フレーム名からフレーム番号を取得する
					int FrameIndex = MV1SearchFrame(_handle, charaMotionData.effectPlay3DFrame.c_str());

					// フレームの現在のワールドでの状態を示す行列を取得する
					MATRIX FrameMatrix = MV1GetFrameLocalWorldMatrix(_handle, FrameIndex);

					//ダウンキャスト
					ModeEffekseer* effect = GetObjectServer()->GetGame()->GetModeEffekseer();
					effect->Play(
						charaMotionData.effectPlayName,
						Vector3D(FrameMatrix.m[3][0], FrameMatrix.m[3][1], FrameMatrix.m[3][2]),
						Vector3D(_eulerAngle.x, _eulerAngle.y + PI, _eulerAngle.z)
					);
				}

				_motCnt++;
				break;
			}
			case MotionCommand::PLAY_SOUND:
			{
				//SEの再生
				gGlobal._sndServer.Get(charaMotionData.soundPlayName)->Play();
				_motCnt++;
				break;
			}
			case MotionCommand::LOOP:
			{
				_motCnt = 0;
				break;
			}
			case MotionCommand::CHANGE_MOTION:
			{
				ChangeState(charaMotionData.ChangeMotion);
				_motCnt = 0;
				break;
			}
			case MotionCommand::MOVE:
			{
				float rad = DegToRad(charaMotionData.vector) + _eulerAngle.y;

				Vector3D v(sin(rad), 0, cos(rad));
				v.Normalized();

				_pos += v * charaMotionData.vectorScale;
		
				_motCnt++;
				break;
			}
			//default:
			//	//エラー
			//	break;
			//}
			}

			//モーションデータのサイズより_motCnt変数が大きくならないなように
			if (_motCnt >= motionData[MotionType::PLAYER][(int)_actionState].size()) {
				_motCnt = 0;
			}
		}
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