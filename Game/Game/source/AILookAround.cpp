#include"AILookAround.h"
#include"AIComponent.h"

AILookAround::AILookAround(AIComponent* owner)
	:AIState(owner)
	,_frameCnt(0)
{

}

AILookAround::~AILookAround() {

}

void AILookAround::OnEnter() {
	_frameCnt = 120;
}

void AILookAround::OnExist() {

}

bool AILookAround::Process() {
	_frameCnt--;

	if (_frameCnt <= 0) {
		_owner->ChangeState("Patrol");
	}

	return true;
}