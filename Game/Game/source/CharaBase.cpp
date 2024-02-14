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
	, _moveCom(NEW MoveComponent(this, 10))
	, _anim(NEW AnimationComponent(this))
{
	server->GetCharas().emplace_back(this);
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
