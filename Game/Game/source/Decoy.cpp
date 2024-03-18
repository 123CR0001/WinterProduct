#include"Decoy.h"
#include"Player.h"
#include"ObjectServer.h"
#include"CapsuleComponent.h"
#include"AnimationComponent.h"

#include"ModeGame.h"
#include"ModeEffekseer.h"

#include"ApplicationGlobal.h"

Decoy::Decoy(Player* player, float rad)
	:ObjectBase(player->GetObjectServer(),false,"Decoy")
	,_capsule(NEW CapsuleComponent(this,1000))
	,_elapsedFrame(0)
	,_frameCnt(0)
	,_maxFrame(300)
{
	//プレイヤーと同じ位置から
	_pos = player->GetPos();
	_eulerAngle = player->GetEulerAngle();
	_eulerAngle.y += rad;

	_capsule->SetMember(100.f, 40.f);
	_capsule->AddSkipName("Decoy");
	_capsule->AddSkipName("CommonSoldier");
	_capsule->AddSkipName("player");
	_capsule->AddSkipName("Tracer");

}

Decoy::~Decoy() {

}

bool Decoy::Initialize() {
	ObjectBase::Initialize();

	_handle = GetObjectServer()->GetGame()->GetModeEffekseer()->Play("Decoy", _pos, _eulerAngle);
	gGlobal._sndServer.Get("SE_08")->Play();

	return true;
}

bool Decoy::Terminate() {
	ObjectBase::Terminate();
	return true;
}

bool Decoy::Process() {
	ObjectBase::Process();

	PhysWorld::CollisionDetectionResult event = _capsule->GetEventOverlapResult();

	//なにかとぶつかったら、反射する
	if (event.isHit) {
		const Vector3 ref = Vector3::Reflect(GetForward(), event.item.normal);
		_eulerAngle.y = atan2f(ref.x, ref.z);
	}

	//移動
	AddPos(GetForward() * 2.0f);

	SetPosPlayingEffekseer3DEffect(_handle, _pos.x, _pos.y + PI, _pos.z);

	SetRotationPlayingEffekseer3DEffect(_handle, _eulerAngle.x, _eulerAngle.y, _eulerAngle.z);

	//再生が終了した
	if (_frameCnt == 20) {
		_handle = GetObjectServer()->GetGame()->GetModeEffekseer()->Play("Decoy", _pos, _eulerAngle);
		_frameCnt = 0;
	}

	_frameCnt++;

	if(_elapsedFrame == _maxFrame) {
		GetObjectServer()->DeleteObject(this);
	}
	_elapsedFrame++;

	return true;
}

bool Decoy::Render() {
	return true;
}