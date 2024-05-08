#include"AILoseSight.h"
#include"AIComponent.h"
#include"ObjectBase.h"

#include"ObjectServer.h"
#include"PhysWorld.h"

#include"ModeGame.h"

AILoseSight::AILoseSight(AIComponent* owner)
	:AIState(owner)
	, _frameCnt(0)
{

}

AILoseSight::~AILoseSight() {

}

void AILoseSight::OnEnter() {
	//LoseSgiht�̎��Ԃ�ݒ肷��
	_frameCnt = 120;
}

void AILoseSight::OnExist() {

}

bool AILoseSight::Process() {
	_frameCnt--;

	//��莞�Ԍo�߂�����BackPatrol�ɃX�e�[�g��ύX����
	if (_frameCnt <= 0) {
		_owner->ChangeState("BackPatrol");
	}

	//BackPatrol�͍ŏ��ɓo�^���ꂽ���W�ɖ߂�̂ŁAPatrol �� LookAround �� Chase(��������CheckPoint) �� LookAround �� BackPatrol�ł��APatrol�ŋL�^���ꂽ���W�ɖ߂�

	//���������������H
	{
		Vector3 p;
		if (_owner->GetOwner()->GetObjectServer()->GetPhysWorld()->IsHear(_owner->GetOwner(), &p)) {
			_owner->AddPoint("MoveTo", p);
			_owner->ChangeState("Discovery");
			_owner->AddPoint("BackPatrolGoal", _owner->GetOwner()->GetPos());
		}
	}

	//�o�^���Ă��閼�O�Ɠ������O�����I�u�W�F�N�g�����E�ɓ��ꂽ��
	{
		auto object = _owner->IsFound();

		if (object) {
			//AIState��ύX
			_owner->AddPoint("MoveTo", object->GetPos());
			_owner->ChangeState("Discovery");
			_owner->AddPoint("BackPatrolGoal", _owner->GetOwner()->GetPos());
		}
	}

	//LightsOut�ɂȂ�����,AIBlindWalk�ɕύX
	if (_owner->GetOwner()->GetObjectServer()->GetGame()->IsUsingLightsOut()) {
		_owner->ChangeState("BlindWalk");
	}

	return true;
}