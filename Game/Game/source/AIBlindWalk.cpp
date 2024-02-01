#include"AIBlindWalk.h"
#include"AIComponent.h"
#include"ObjectBase.h"
#include"CommonSoldier.h"
#include"ObjectServer.h"
#include"MoveComponent.h"
#include<algorithm>

AIBlindWalk::AIBlindWalk(AIComponent* owner)
	:AIState(owner)
	,_rotateSpeed(DegToRad(1.f))
	,_rotateAngle(0.f)
	,_moveDist(0.f)
{

}

AIBlindWalk::~AIBlindWalk(){}

void AIBlindWalk::OnEnter(){
	_moveDist = 1100.f;
}

void AIBlindWalk::OnExist(){}

bool AIBlindWalk::Process() {

	auto owner = _owner->GetOwner();
	auto iter = std::find(owner->GetObjectServer()->GetCommonSoldiers().begin(), owner->GetObjectServer()->GetCommonSoldiers().end(), owner);

	//“o˜^‚³‚ê‚Ä‚¢‚é‚©
	if (iter == owner->GetObjectServer()->GetCommonSoldiers().end()) { return false; }
	if (_moveDist < 0.01f) { return true; }

	//if(_moveDist < 0.001f){_owner->ChangeState("");}

	float moveSpeed = 2.f;
	float rotateSpeed = DegToRad((float)(rand() % 2));

	if (rand() % 2) { 
		rotateSpeed *= -1.f;
	}

	(*iter)->GetMoveComponent()->SetRotateSpeed(rotateSpeed);
	
	_moveDist -= moveSpeed;

	(*iter)->GetMoveComponent()->SetMoveSpeed(moveSpeed);
		
	return true;
}