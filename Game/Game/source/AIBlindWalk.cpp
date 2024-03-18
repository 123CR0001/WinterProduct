#include"AIBlindWalk.h"
#include"AIComponent.h"
#include"ObjectBase.h"
#include"CommonSoldier.h"
#include"ObjectServer.h"
#include"MoveComponent.h"
#include"CapsuleComponent.h"
#include<algorithm>

AIBlindWalk::AIBlindWalk(AIComponent* owner)
	:AIState(owner)
	,_rotateSpeed(1)
	,_rotateAngle(0)
	,_moveDist(0.f)
{

}

AIBlindWalk::~AIBlindWalk(){}

void AIBlindWalk::OnEnter(){
	//�ړ����鋗��
	_moveDist = 1100.f;

	//�ǂ���ɉ�]���邩
	if (rand() % 2) { _rotateSpeed *= -1; }

	//�ړ�����ő��
	_rotateAngle = rand() % 360;

	_waitCnt = 0;

	_moveSpeed = 2.f;

}

void AIBlindWalk::OnExist(){}

bool AIBlindWalk::Process() {

	//�ړ����x
	float moveSpeed = _moveSpeed;

	//���������������H
	if (_owner->GetOwner()->GetObjectServer()->GetPhysWorld()->IsHear(_owner->GetOwner())) {
		if (rand() % 2) { _owner->ChangeState("Panic"); }
		else { _moveSpeed = 2.f * 2.f; }
	}

	//��莞�Ԏ~�܂�
	if (_waitCnt > 0) { _waitCnt--; return true; }

	//�ړ����鋗����0�ł͂Ȃ���
	if (_moveDist < 0.01f) { return true; }

	//��]
	if (_rotateAngle > 0) { 
		const Vector3 addAngle(0.f, DegToRad(static_cast<float>(_rotateSpeed)), 0.f);
		_owner->GetOwner()->AddEulerAngle(addAngle);
	}
	//��]������A��]�ʂ����炷
	_rotateAngle -= abs(_rotateSpeed);

	//�ړ�
	_owner->GetOwner()->AddPos(_owner->GetOwner()->GetForward() * moveSpeed);

	_moveDist -= moveSpeed;

	//�ړ�������0�ɂȂ���
	if (_moveDist < 0.001f) { _owner->ChangeState(GetName()); }

	//////�I�u�W�F�N�g�ɂԂ�������A��莞�Ԏ~�܂�B���̌�A90�x��]���āA�ʕ����ɐi��
	//if ((*iter)->GetCapsuleComponent()->GetOverlapResult().isHit && _rotateAngle <= 0) {
	//	_rotateAngle = 90;
	//	_waitCnt = 60;
	//}
		
	return true;
}