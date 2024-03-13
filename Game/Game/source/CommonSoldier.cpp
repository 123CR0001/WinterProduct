#include"CommonSoldier.h"
#include"Player.h"
#include"ObjectServer.h"

#include<algorithm>

#include"PhysWorld.h"

#include"MoveComponent.h"
#include"CapsuleComponent.h"
#include"FrameComponent.h"
#include"CameraComponent.h"
#include"CameraZoomComponent.h"

#include"ModeGame.h"
#include"ModeEffekseer.h"

#include"AIComponent.h"
#include"AIPatrol.h"
#include"AIBackPatrol.h"
#include"AIMoveTo.h"
#include"AIBlindWalk.h"
#include"AIPanic.h"
#include"AILookAround.h"
#include"AIDeath.h"
#include"AIStay.h"
#include"AIDiscovery.h"
#include"AIAttack.h"

#include"CommonSoldierAnimaitonComponent.h"
#include"CountKillComboComponent.h"
#include"CameraZoomComponent.h"

#include"LightsOut.h"
#include"TimeLine.h"

#include"MyUIServer.h"
#include"UI.h"

constexpr int SIDE_NUM = 100;

CommonSoldier::CommonSoldier(ObjectServer* server) 
	:CharaBase(server,"CommonSoldier")
	, _AI(NEW AIComponent(this, 11))
	,_capsule(NEW CapsuleComponent(this,1000))
	,_anim(NEW CommonSoldierAnimationComponent(this,1000))
	,_detectionLevel(0.f)
	,_isAttack(false)
{
	NEW CountKillComboComponent(this);

	//AStateの登録
	_AI->RegisterState(NEW AIBackPatrol(_AI));
	_AI->RegisterState(NEW AIPatrol(_AI));
	_AI->RegisterState(NEW AIBlindWalk(_AI));
	_AI->RegisterState(NEW AIMoveTo(_AI));
	_AI->RegisterState(NEW AIPanic(_AI));
	_AI->RegisterState(NEW AILookAround(_AI));
	_AI->RegisterState(NEW AIDeath(_AI));
	_AI->RegisterState(NEW AIStay(_AI));
	_AI->RegisterState(NEW AIDiscovery(_AI));
	_AI->RegisterState(NEW AIAttack(_AI));

	server->GetCommonSoldiers().emplace_back(this);
	server->GetEnemys().emplace_back(this);

	for(int a = 0; a < SIDE_NUM; a++) {
		_versNums.emplace_back(0);
		_versNums.emplace_back(a + 1);
		_versNums.emplace_back(a + 2);
	}

	_visionHandleRed = ResourceServer::LoadGraph("res/UI/visualrange_01.png");
	_visionHandleBlue = ResourceServer::LoadGraph("res/UI/visualrange_02.png");

	_capsule->AddSkipName("CommonSoldier");

	server->GetGame()->IncrementEnemyCount();
}

CommonSoldier::~CommonSoldier(){
}

bool CommonSoldier::Initialize() {
	ObjectBase::Initialize();

	// 3Dモデルの1番目のアニメーションをアタッチする
	_attachIndex = 0;
	// アタッチしたアニメーションの総再生時間を取得する

	_isStand = true;

	//カプセルのメンバを設定
	_capsule->SetMember(180.f, 30.f);

	//AIの視覚のメンバを設定
	_AI->SetViewAngle(120.f);
	_AI->SetViewDist(500.f);
	_AI->SetView(Vector3D(0.f, 100.f, 0.f));

	_detectionLevel = 0.f;

	//最初のAI状態
	_AI->ChangeState("Patrol");

	return true;
}

bool CommonSoldier::Terminate() {
	CharaBase::Terminate();

	auto iter = std::find(GetObjectServer()->GetCommonSoldiers().begin(), GetObjectServer()->GetCommonSoldiers().end(), this);
	if (iter != GetObjectServer()->GetCommonSoldiers().end()) {
		GetObjectServer()->GetCommonSoldiers().erase(iter);
	}

	return true;
}

bool CommonSoldier::Process() {

	ObjectBase::Process();

	//角度の修正
	if (_eulerAngle.y > 2 * PI) {
		_eulerAngle.y = 0;
	}
	if (_eulerAngle.y <  0) {
		_eulerAngle.y = 2 * PI;
	}

	//検知度の増減(百分率)
	if (_AI->IsFound(GetObjectServer()->GetPlayer()) && GetObjectServer()->GetGame()->GetLightsOut()->IsUse()) {
		//AIごとに上昇するかしないかを分けるかもしれない
		//ここに書いているのは走り書き
		const float dist = _AI->GetViewDist();

		float per = 100.f;// Vector3D::LengthSquare(GetObjectServer()->GetPlayer()->GetPos(), _pos) / dist * dist;
		per = -(per - 1);

		//検知度の上昇
		_detectionLevel += 1.f * 0.01;
	}
	else {
		_detectionLevel -= 0.01f;
	}

	//検知度が100%になった
	if(_detectionLevel >= 1.f && !_isAttack) {
		{
			//検知度のUIを削除
			auto ui = GetObjectServer()->GetGame()->GetUIServer()->Get("DetectionLavel");

			if(ui) {
				GetObjectServer()->GetGame()->GetUIServer()->DeleteUI(ui);
			}
		}

		_AI->ChangeState("Attack");

		NEW CameraZoomComponent(GetObjectServer()->GetPlayer()->GetCamera(), 0.6f, 60);

		//攻撃する
		_isAttack = true;

		//1フレーム後に、キャラクター死亡
		auto timeLine = GetObjectServer()->GetGame()->GetTimeLine();
		timeLine->AddLine(
			1, 
			[=]()mutable {
				GetObjectServer()->GetPlayer()->ApplyDamage(
					DamageData{
						true,this,PhysWorld::CollisionDetectionItem{}
					}
				);
			}
		);
	}

	_detectionLevel = Clamp(0.f, 1.f, _detectionLevel);


	//ダメージを受けたか
	if (_damageData.isDamage) {

		//死亡
		_AI->ChangeState("Death");

		auto iter2 = std::find(GetObjectServer()->GetEnemys().begin(), GetObjectServer()->GetEnemys().end(), this);
	
		if (iter2 != GetObjectServer()->GetEnemys().end()){
			GetObjectServer()->GetEnemys().erase(iter2);
		}

		//エフェクト再生
		GetObjectServer()->GetGame()->GetModeEffekseer()->Play(
			"Blood01",
			_damageData.item.hitPosition,
			Vector3D(0.f, atan2f(_damageData.item.pushVec.x, _damageData.item.pushVec.z), 0.f)
		);
		GetObjectServer()->GetGame()->GetModeEffekseer()->Play(
			"Blood02",
			_pos + Vector3D(0.f, 10.f, 0.f),
			_eulerAngle
		);
		//カメラの揺れ
		GetObjectServer()->GetPlayer()->GetCamera()->Swap();

		_state = STATE::kDead;

		DeleteComponent(_capsule);

		//_enemyCoutを減らす
		GetObjectServer()->GetGame()->DecrementEnemyCount();

		//プレイヤーの移動スピードの倍率を上げる
		GetObjectServer()->GetPlayer()->AddMoveSpeedMag(0.2f);

		//ライツアウトの制限時間を伸ばす
		GetObjectServer()->GetGame()->GetLightsOut()->AddFrameCount();

		//データを空にする
		_damageData = DamageData{};
	}

	
	if(GetObjectServer()->GetGame()->GetResultData()->maxDetectionLevel < _detectionLevel){
		GetObjectServer()->GetGame()->GetResultData()->maxDetectionLevel = _detectionLevel;
	}


	return true;
}

bool CommonSoldier::Render() {
	CharaBase::Render();

	//キャラクターがむいている角度から、
	float rot = _eulerAngle.y - DegToRad(_AI->GetViewAngle()) / 2.f;

	//前フレームの頂点データを削除
	_vers.clear();

	//床と重ならないように、足元よりちょっと上に位置を調整
	Vector3D pos = _pos + Vector3D(0.f, 1.f, 0.f);

	int handle = _visionHandleRed;
	COLOR_U8 dif = GetColorU8(255, 0, 0, 0);
	COLOR_U8 spc = GetColorU8(255, 0, 0, 0);

	if(_AI->IsFound(GetObjectServer()->GetPlayer())) {
		handle = _visionHandleBlue;
		dif = GetColorU8(0, 0, 255, 0);
		spc = GetColorU8(0, 0, 255, 0);
	}

	//中心点
	_vers.emplace_back(
		VERTEX3D{
			DxConverter::VecToDx(Vector3D(pos)),
			VGet(0.f,1.f,0.f),
			dif,
			spc,
			0.5f,
			0.5f,
			0.f,
			0.f
		}
	);

	for(int a = 0; a <= SIDE_NUM; a++) {

		Vector3D viewPos = pos + Vector3D(sinf(rot), 0.f, cosf(rot) ) * _AI->GetViewDist();

		for(auto&& frame : GetObjectServer()->GetPhysWorld()->GetFrameComponent()) {
			MV1_COLL_RESULT_POLY result = 
				MV1CollCheck_Line(
					frame->GetOwner()->GetHandle(),
					frame->GetOwner()->GetAttachIndex(),
					DxConverter::VecToDx(pos),
					DxConverter::VecToDx(viewPos)
				);

			if(result.HitFlag) { viewPos = DxConverter::DxToVec(result.HitPosition); }
		}

		VERTEX3D ver = {
			DxConverter::VecToDx(viewPos),
			VGet(0.f,1.f,0.f),
			dif,
			spc,
			0.5f + 0.5f * (Vector3D::Length(viewPos,pos) / _AI->GetViewDist()),
			0.5f + 0.5f * (Vector3D::Length(viewPos,pos) / _AI->GetViewDist()),
			0.f,
			0.f
		};

		_vers.emplace_back(ver);

		rot += DegToRad(_AI->GetViewAngle()) / SIDE_NUM;

	}


	SetUseLighting(FALSE);
	DrawPolygonIndexed3D(_vers.data(), _vers.size(), _versNums.data(), _versNums.size() / 3, handle,FALSE);
	SetUseLighting(TRUE);

	return true;
}

void CommonSoldier::SetJsonDataUE(nlohmann::json data) {

	AIState* state = _AI->GetStateMap("Patrol");

	if (!state) { return; }

	//AIPatrolの巡回経路を登録
	for (auto&& marker : data) {

		std::string name = marker.at("objectName");
		//オブジェクト名＋数字		例：Enemy1
		//数字部分を削除

		//0~9の数字が含まれていれば、それ以降の文字を削除
		for(int a = 0; a < 10; a++) {
			//整数をstringに変換　数字を検索
			int num = name.find(std::to_string(a));

			//findは、検索した文字がなければ、-1を返す
			if(num != -1) { name = name.substr(0, num); break; }
		}

		const Vector3D pos(marker.at("translate").at("x"), marker.at("translate").at("z"), -1 * marker.at("translate").at("y"));
		_AI->AddPoint(state->GetName(), pos);
		
	}
	if (_AI->GetPoints(state->GetName()).size() > 0) {
		SetPos(_AI->GetPoints(state->GetName()).front());
		const Vector3D deg = Vector3D(data.front().at("rotate").at("x"), data.front().at("rotate").at("z"), data.front().at("rotate").at("y"));
		SetEulerAngleDeg(deg);
	}

	ModelMatrixSetUp();

}