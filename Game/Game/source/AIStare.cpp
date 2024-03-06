#include"AIStare.h"
#include"AIComponent.h"

#include"ObjectBase.h"

AIStare::AIStare(AIComponent* owner)
	:AIState(owner)
{

}

AIStare::~AIStare(){}

void AIStare::OnEnter() {

}

void AIStare::OnExist() {

}

bool AIStare::Process() {

	if(!_owner->IsFound(_owner->GetChaseObject())) {
		_owner->ChangeState("LookAround");
	}

	return true;
}