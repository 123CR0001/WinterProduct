#include"AIPatrol.h"
#include"AIComponent.h"
#include"ObjectBase.h"
#include"CommonSoldier.h"
#include"ObjectServer.h"
#include<algorithm>
#include"AIBackPatrol.h"
#include"PhysWorld.h"
#include"Player.h"

AIPatrol::AIPatrol(AIComponent* owner)
	:AIState(owner)
	,_patrolPointsNum(1)
{}

AIPatrol::~AIPatrol(){}


void AIPatrol::OnEnter() {
}

void AIPatrol::OnExist() {

}

bool AIPatrol::Process() {

	auto owner = _owner->GetOwner();
	auto objectServer = _owner->GetOwner()->GetObjectServer();

	//�o�H������
	_owner->MoveTo(_owner->GetPoints(GetName()), _patrolPointsNum);

	//���������������H
	Vector3D p;
	if (_owner->GetOwner()->GetObjectServer()->GetPhysWorld()->IsHear(_owner->GetOwner(), &p)) {
		_owner->AddPoint("CheckPoint", p);
		_owner->ChangeState("CheckPoint");
	}

	//�v���C���[����������
	if (_owner->IsFound(objectServer->GetPlayer())) {
		_owner->ChangeState("ChasePlayer");
	}

	//LightsOut�ɂȂ�����,AIBlindWalk�ɕύX
	if (ModeServer::GetInstance()->IsAdd("LightsOut")) {
		_owner->ChangeState("BlindWalk");
	}
	
	return true;
}