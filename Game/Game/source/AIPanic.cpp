#include"AIPanic.h"
#include"AIComponent.h"

AIPanic::AIPanic(AIComponent* owner)
	:AIState(owner) {

}

AIPanic::~AIPanic() {

}

void AIPanic::OnEnter() {
	// パニック状態の時間を設定
	_frameCnt = 180;
}

void AIPanic::OnExist() {

}

bool AIPanic::Process() {

	_frameCnt--;

	//パニック状態が終わったら、BlindWalk状態に遷移
	if (_frameCnt == 0) {
		_owner->ChangeState("BlindWalk");
	}

	return true;
}