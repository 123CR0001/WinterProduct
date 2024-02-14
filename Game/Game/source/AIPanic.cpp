#include"AIPanic.h"
#include"AIComponent.h"

AIPanic::AIPanic(AIComponent* owner)
	:AIState(owner) {

}

AIPanic::~AIPanic() {

}

void AIPanic::OnEnter() {
	_frameCnt = 180;
}

void AIPanic::OnExist() {

}

bool AIPanic::Process() {

	_frameCnt--;

	if (_frameCnt == 0) {
		_owner->ChangeState("BlindWalk");
	}

	return true;
}