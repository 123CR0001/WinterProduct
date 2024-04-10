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

	//モーションデータを参照してする処理
	_motCom = NEW MotionComponent(_anim, 1000);
	
	//オブジェクトサーバーにプレイヤークラス登録用ポインタ変数に自身のアドレスを登録
	server->SetPlayer(this);

	//このクラス特有のモーションデータのコマンド処理
	auto func = [this](const MOTION_DATA_ITEM& item) {_weapon->OnAttack(); _motCom->IncrementMotionCount(); };
	auto func2 = [this](const MOTION_DATA_ITEM& item) {_weapon->OffAttack(); _motCom->IncrementMotionCount();  };
	auto func3 = [this](const MOTION_DATA_ITEM& item) {ChangeState(item.ChangeMotion); };

	//上記の処理をMotionComponentに追加
	_motCom->RegisterCustomCommand("ATTACK_ON",func);
	_motCom->RegisterCustomCommand("ATTACK_OFF", func2);
	_motCom->RegisterCustomCommand("CHANGE_MOTION", func3);

	//リザルトのクリアタイムを集計
	NEW CountClearTimeComponent(this);

	_weapon->SetFrameName("Owl_RightHand");
}

Player::~Player() {

}

bool Player::Initialize() {
	// モデルデータのロード（テクスチャも読み込まれる）
	CharaBase::Initialize();

	//カメラの初期化
	_cameraCom->Initialize();

	//カプセルの設定
	_capsule->SetMember(40.f, 30.f);
	_capsule->AddSkipName("Decoy");

	//状態を”kIdle”に、アニメーションも同様
	_actionState = ACTION_STATE::kIdle;
	_anim->ChangeAnimation("Idle");

	//カメラと同じ方向を向く
	Vector3 vec(_pos - _cameraCom->GetPos());
	vec.Normalized();
	_eulerAngle.y = atan2f(vec.x, vec.z);

	//デコイの残り使用回数のUI
	_decoyTimesText->LoadDivNumber("res/UI/Result/ui_timer_01.png", 5, 2, 46, 70);
	_decoyTimesText->SetPos(Vector2(337.f * SCREEN_WIDTH_MAG, 1004.5f * SCREEN_HEIGHT_MAG));
	_decoyTimesText->SetSize(Vector2(40.f * SCREEN_WIDTH_MAG, 76 * SCREEN_WIDTH_MAG));

	GetObjectServer()->GetGame()->GetUIServer()->AddUI(NEW UISpriteText(_decoyTimesText, 100));

	//上記の背景
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

	//モデルに今の設定を反映
	ModelMatrixSetUp();

	return true;
}

bool Player::Terminate() {
	CharaBase::Terminate();

	//オブジェクトサーバーにプレイヤークラス登録用ポインタ変数をnullptrにする
	GetObjectServer()->SetPlayer(nullptr);
	return true;
}

bool Player::Process() {

	//ゲームパッドの入力値
	auto pad = GetObjectServer()->GetGame()->GetPad();

	//トリガー値
	auto trg = pad->GetTrgButton();

	//移動スピード
	const float moveSpeed = 5.f * _moveSpeedMag;

	//入力処理
	switch (_actionState) {
	case ACTION_STATE::kIdle:
	case ACTION_STATE::kWalk: {
		_actionState = ACTION_STATE::kIdle;

		float angle = 0.f;

		if(pad->IsInputLeftStick()) {
			angle = pad->GetLeftStickAtan2();

			//プレイヤーからカメラのY軸の角度を反転
			_eulerAngle.y = -_cameraCom->GetAngle().y;

			//移動速度、角度を登録
			_moveCom->SetRotateSpeed(angle);
			_moveCom->SetMoveSpeed(moveSpeed);

			//状態を”kWalk”に
			_actionState = ACTION_STATE::kWalk;

		}

		//攻撃(パターン2)
		if(trg & INPUT_X && _isLightsOut) {
			_actionState = ACTION_STATE::kAttack2;

			gGlobal._sndServer.Get("SE_02")->Play();
		}
		//攻撃(パターン3)
		if(trg & INPUT_A && _isLightsOut) {
			_actionState = ACTION_STATE::kAttack3;
			gGlobal._sndServer.Get("SE_02")->Play();
		}

		//デコイの使用
		if(trg & INPUT_X && !_isLightsOut && _decoyTimes > 0) {
			NEW Decoy(this, angle);
			_decoyTimes--;
		}

		//状態を”kSilent”に
		if(trg & INPUT_A && !_isLightsOut) {
			_actionState = ACTION_STATE::kSilent;
		}
		break;
	}
	case ACTION_STATE::kSilent:
	case ACTION_STATE::kSilentWalk:
		_actionState = ACTION_STATE::kSilent;

		//”kIdle”状態の移動と処理は同じ
		if (pad->IsInputLeftStick()) {

			_eulerAngle.y = -_cameraCom->GetAngle().y;

			float angle = atan2f((float)pad->GetLeftStick().x, (float)pad->GetLeftStick().y);

			_moveCom->SetRotateSpeed(angle);

			//移動速度が半減
			_moveCom->SetMoveSpeed(moveSpeed / 2.f);

			_actionState = ACTION_STATE::kSilentWalk;

		}

		//状態を”kIdle”に
		if (trg & INPUT_A) {
			_actionState = ACTION_STATE::kIdle;
		}

		//ライツアウトになったら、状態を戻す
		if (_isLightsOut) {
			_actionState = ACTION_STATE::kIdle;
		}
		break;
	}

	ObjectBase::Process();
	//死亡処理
	//一度だけ
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

	//足音
	if (_moveCom->GetSpeed() >= moveSpeed && !ModeServer::GetInstance()->IsAdd("LightsOut")) {
		new SoundComponent(this, 200.f);
	}	

	//ダメージを受けた
	if(_damageData.isDamage) {
		//状態を”kDead”に
		_state = STATE::kDead;

		//空にする
		_damageData = DamageData{};
	}

	//ライツアウトを使用しているなら、デコイの使用回数を描画するUIを非表示に
	if(_isLightsOut) {
		_decoyTimesText->SetAlpha(0.f);
	}

	//UIの更新
	_decoyTimesText->SetNumber(_decoyTimes);

	//3Dサウンドの情報を更新
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