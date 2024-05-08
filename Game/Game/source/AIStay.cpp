#include"AIStay.h"
#include"AIComponent.h"
#include"ObjectBase.h"
#include"ObjectServer.h"

#include"ModeGame.h"

AIStay::AIStay(AIComponent* owner)
	:AIState(owner)
	,_isSetForward(false)
{

}

AIStay::~AIStay(){}

void AIStay::OnEnter() {
	//���ł��p�x��ݒ肵����A���̊p�x�����̃X�e�[�g�ɂȂ����A���̊p�x�ɕω�����
	if (!_isSetForward) { _ownerForward = _owner->GetOwner()->GetForward(); _isSetForward = true; }
}

void AIStay::OnExist() {

}

bool AIStay::Process() {

	//���������������H
	{
		Vector3 p;
		if(_owner->GetOwner()->GetObjectServer()->GetPhysWorld()->IsHear(_owner->GetOwner(), &p)) {
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

	//LightsOut�ɂȂ�����AAIBlindWalk�ɕύX
	if(_owner->GetOwner()->GetObjectServer()->GetGame()->IsUsingLightsOut()) {
		_owner->ChangeState("BlindWalk");
	}

	//�ݒ肳��Ă���(_ownerForward)�̊p�x�֏���������
	float angle = Vector3::CrossAngleXZ(_owner->GetOwner()->GetForward(), _ownerForward);

	//�p�x�����ȉ��ɂȂ�����A�������I��
	if(fabsf(angle) < DegToRad(3)) { return true; }

	//�p�x���v���X���}�C�i�X���ŉ�]������ς���
	if(angle < 0.f) {
		_owner->GetOwner()->AddEulerAngle(Vector3(0.f, DegToRad(1), 0.f));
	}
	else {
		_owner->GetOwner()->AddEulerAngle(Vector3(0.f, -DegToRad(1), 0.f));
	}

	return true;
}