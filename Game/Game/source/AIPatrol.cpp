#include"AIPatrol.h"
#include"AIComponent.h"
#include"ObjectBase.h"
#include"CommonSoldier.h"
#include"ObjectServer.h"
#include<algorithm>
#include"AIBackPatrol.h"
#include"PhysWorld.h"

AIPatrol::AIPatrol(AIComponent* owner, AIBackPatrol* AI)
	:AIState(owner)
	,_AIBackPatrol(AI)
	,_patrolPointsNum(1)
{}

AIPatrol::~AIPatrol(){}


void AIPatrol::OnEnter() {
	_AIBackPatrol->ClearBackPatrol();
}

void AIPatrol::OnExist() {

}

bool AIPatrol::Process() {

	if (_patrolPoints.size() == 0) {
		//ó‘Ô‚ð‘JˆÚ‚·‚é
		//owner->ChangeState("GateKeeper");
	}

	//Server‚Ì_enemys‚É“o˜^‚³‚ê‚Ä‚¢‚é‚©
	auto iter = std::find(_owner->GetOwner()->GetObjectServer()->GetCommonSoldiers().begin(),
		_owner->GetOwner()->GetObjectServer()->GetCommonSoldiers().end(),
		_owner->GetOwner()
	);

	//‚³‚ê‚Ä‚¢‚½‚çAˆ—‚ð‚·‚é
	if (iter != _owner->GetOwner()->GetObjectServer()->GetCommonSoldiers().end()) {
		(*iter)->MoveRoute(_owner->GetPoints(GetName()), _patrolPointsNum);
		if ((*iter)->IsPlayerFound()) {
			_owner->ChangeState("ChasePlayer");
		}
	}

	//‰¹‚ª•·‚±‚¦‚½‚©H
	Vector3D p;
	if (_owner->GetOwner()->GetObjectServer()->GetPhysWorld()->IsHear(_owner->GetOwner(), p)) {
		_owner->AddPoint("CheckPoint", p);
		_owner->ChangeState("CheckPoint");
	}
	
	return true;
}