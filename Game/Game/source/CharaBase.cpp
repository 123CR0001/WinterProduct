#include"CharaBase.h"
#include"ObjectServer.h"
#include"ModeGame.h"
#include"AnimationConmponent.h"
#include"CapsuleComponent.h"
#include<algorithm>
#include"appframe.h"
#include"FrameComponent.h"
#include"MoveComponent.h"

CharaBase::CharaBase(ObjectServer* server, std::string name)
	:ObjectBase(server, false, name)
	, _anim(NEW AnimationComponent(this,100))
	,_capsule(NEW CapsuleComponent(this))
	, _moveCom(NEW MoveComponent(this, 10))
{
	server->GetCharas().emplace_back(this);
	_capsule->SetMember(170.f, 40.f);

}

CharaBase::~CharaBase() {
	
}

bool CharaBase::ChangeState(std::string stateName) {
	return true;
}

bool CharaBase::Initialize() {
	ObjectBase::Initialize();

	_isStand = false;

	return true;
}

bool CharaBase::Terminate(){

	ObjectBase::Terminate();

	auto iter = std::find(
		GetObjectServer()->GetCharas().begin(), GetObjectServer()->GetCharas().end(), this);
	if (iter != GetObjectServer()->GetCharas().end()) {
		GetObjectServer()->GetCharas().erase(iter);
	}

	return true;
}

bool CharaBase::Render() {

	ModelMatrixSetUp();
	MV1DrawModel(_handle);

	return true;
}

void CharaBase::FixPos() {
	//オブジェクトとの押出処理
	int i = 0;
	while (1) {
		PhysWorld::CollisionDetectionResult result = _capsule->GetOverlapResult();

		if (result.isHit) {
			_pos += result.item.pushVec;
		}
		else { break; }
		i++;
		//20回以上判定をしたら、break
		if (i > 20) {
			break;
		}
	}
}
