#include"AIDeath.h"
#include"AIComponent.h"
#include"ObjectBase.h"
#include"ObjectServer.h"

AIDeath::AIDeath(AIComponent* owner)
	:AIState(owner)
	, _frameCnt(0) 
{

}

AIDeath::~AIDeath(){}

void AIDeath::OnEnter() {
	_frameCnt = 180;
}

void AIDeath::OnExist() {

}

bool AIDeath::Process() {

	_frameCnt--;
	//フレーム数が0以下になったら
	if (_frameCnt <= 0) {
		//削除
		_owner->GetOwner()->GetObjectServer()->DeleteObject(_owner->GetOwner());
	}

	return true;
}