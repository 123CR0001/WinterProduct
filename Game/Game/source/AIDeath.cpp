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
	//�t���[������0�ȉ��ɂȂ�����
	if (_frameCnt <= 0) {
		//�폜
		_owner->GetOwner()->GetObjectServer()->DeleteObject(_owner->GetOwner());
	}

	return true;
}