#include"AIPanic.h"
#include"AIComponent.h"

AIPanic::AIPanic(AIComponent* owner)
	:AIState(owner) {

}

AIPanic::~AIPanic() {

}

void AIPanic::OnEnter() {
	// �p�j�b�N��Ԃ̎��Ԃ�ݒ�
	_frameCnt = 180;
}

void AIPanic::OnExist() {

}

bool AIPanic::Process() {

	_frameCnt--;

	//�p�j�b�N��Ԃ��I�������ABlindWalk��ԂɑJ��
	if (_frameCnt == 0) {
		_owner->ChangeState("BlindWalk");
	}

	return true;
}