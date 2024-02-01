#include"AIRandomWalk.h"
#include"AIComponent.h"
#include"ObjectBase.h"
#include"CommonSoldier.h"
#include"ObjectServer.h"
#include"MoveComponent.h"
#include<algorithm>

AIRandomWalk::AIRandomWalk(AIComponent* owner)
	:AIState(owner) {

}

AIRandomWalk::~AIRandomWalk(){}

void AIRandomWalk::OnEnter(){
	_rotateAngle = rand() % 60;
	if (rand() % 2) {
		_rotateAngle *= -1;
	}

	_moveDist = 200.f;
}

void AIRandomWalk::OnExist(){}

bool AIRandomWalk::Process() {

	auto owner = _owner->GetOwner();
	auto iter = std::find(owner->GetObjectServer()->GetCommonSoldiers().begin(), owner->GetObjectServer()->GetCommonSoldiers().end(), owner);

	if (iter != owner->GetObjectServer()->GetCommonSoldiers().end()) { return false; }

	//if(_moveDist < 0.001f){_owner->ChangeState("");}

	float rotateSpeed = DegToRad(1.f);
	float moveSpeed = 4.f;
	
	if (fabsf(_rotateAngle) > 0.1f) {
		//回転する角度がマイナスだったら、回転向きを反転させる
		if (_rotateAngle < 0) {
			rotateSpeed *= -1;
		}
		_rotateAngle -= rotateSpeed * -1;
	}
	
	_moveDist -= moveSpeed;
	
	(*iter)->GetMoveComponent()->SetRotateSpeed(rotateSpeed);
	(*iter)->GetMoveComponent()->SetMoveSpeed(moveSpeed);
		
	

	return true;
}