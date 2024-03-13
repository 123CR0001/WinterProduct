#include"AIAttack.h"
#include"AIComponent.h"

#include"Player.h"
#include"ObjectBase.h"
#include"ObjectServer.h"

#include"appframe.h"
AIAttack::AIAttack(AIComponent* owner)
	:AIState(owner)
	,_angle(0.f)
{

}

AIAttack::~AIAttack(){}

void AIAttack::OnEnter(){




}

void AIAttack::OnExist(){}

bool AIAttack::Process() {

	auto forward = _owner->GetOwner()->GetForward().Normalize();
	auto playerPos = _owner->GetOwner()->GetObjectServer()->GetPlayer()->GetPos();
	auto playerToOwner = playerPos - _owner->GetOwner()->GetPos();
	playerToOwner.Normalized();

	_angle = Vector3D::CrossAngleXZ(forward, playerToOwner);

	auto ownerAngle = _owner->GetOwner()->GetEulerAngle();

	if(fabsf(_angle) < DegToRad(10.f)) { return true; }

	if(_angle > 0.f) {

		ownerAngle.y -= DegToRad(3.f);

		_owner->GetOwner()->SetEulerAngle(ownerAngle);

	}
	else {

		ownerAngle.y += DegToRad(3.f);

		_owner->GetOwner()->SetEulerAngle(ownerAngle);

	}

	return true;
}
