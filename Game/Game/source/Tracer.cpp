#include"Tracer.h"
#include"ObjectServer.h"

#include"AIComponent.h"
#include"AITrace.h"
#include"AIRush.h"
#include"AIDeath.h"

#include"MoveComponent.h"
#include"AnimationComponent.h"
#include"CapsuleComponent.h"

#include"Player.h"

#include"TracerSpawner.h"

#include"ModeGame.h"
#include"ModeEffekseer.h"
#include"CameraComponent.h"
#include"TracerAnimationComponent.h"

Tracer::Tracer(TracerSpawner* spawner)
	:CharaBase(spawner->GetObjectServer(), "Tracer")
	, _capsule(NEW CapsuleComponent(this, 1000))
	,_AI(NEW AIComponent(this,1))
	,_anim(NEW TracerAnimationComponent(this,1000))
	,_frameCnt(0)
	,_maxFrame(3)
{

	_attachIndex =  MV1SearchFrame(_handle, "Soldier");

	//AIStateを登録
	_AI->RegisterState(NEW AITrace(_AI));
	_AI->RegisterState(NEW AIRush(_AI));
	_AI->RegisterState(NEW AIDeath(_AI));

	_AI->ChangeState("Trace");

	//今まで記録していた、プレイヤーの座標をmove
	_AI->GetPoints("Trace") = std::move(spawner->GetPoints());

	//最初の座標に移動
	_pos = _AI->GetPoints("Trace").front();

	GetObjectServer()->GetEnemys().emplace_back(this);

	//_enemyCoutを増やす
	spawner->GetObjectServer()->GetGame()->IncrementEnemyCount();

}

Tracer::~Tracer(){

}

bool Tracer::Initialize() {

	//カプセルのメンバを設定
	_capsule->SetMember(180.f, 30.f);

	//AIの視覚のメンバを設定
	_AI->SetViewAngle(120.f);
	_AI->SetViewDist(500.f);
	_AI->SetView(Vector3(0.f, 100.f, 0.f));

	return true;
}

bool Tracer::Terminate() {
	CharaBase::Terminate();
	return true;
}

bool Tracer::Process() {
	ObjectBase::Process();

	if (_frameCnt == _maxFrame) {

		_frameCnt = 0;

		_AI->AddPoint("Trace", GetObjectServer()->GetPlayer()->GetPos());
	}

	_frameCnt++;

	//ダメージを受けたか
	if (_damageData.isDamage) {

		//死亡
		_AI->ChangeState("Death");

		auto iter2 = std::find(GetObjectServer()->GetEnemys().begin(), GetObjectServer()->GetEnemys().end(), this);

		if (iter2 != GetObjectServer()->GetEnemys().end()) {
			GetObjectServer()->GetEnemys().erase(iter2);
		}

		//エフェクト再生
		GetObjectServer()->GetGame()->GetModeEffekseer()->Play(
			"Blood01",
			_damageData.item.hitPosition,
			Vector3(0.f, atan2f(_damageData.item.pushVec.x, _damageData.item.pushVec.z), 0.f)
		);
		GetObjectServer()->GetGame()->GetModeEffekseer()->Play(
			"Blood02",
			_pos + Vector3(0.f, 10.f, 0.f),
			_eulerAngle
		);
		//カメラの揺れ
		GetObjectServer()->GetPlayer()->GetCamera()->Swap();

		DeleteComponent(_capsule);

		//_enemyCoutを減らす
		GetObjectServer()->GetGame()->DecrementEnemyCount();

		//プレイヤーの移動スピードの倍率を上げる
		GetObjectServer()->GetPlayer()->AddMoveSpeedMag(0.2f);

		//データを空にする
		_damageData = DamageData{};
	}

	return true;
}