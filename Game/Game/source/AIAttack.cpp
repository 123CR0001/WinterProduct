#include"AIAttack.h"
#include"AIComponent.h"

#include"Player.h"
#include"ObjectBase.h"
#include"ObjectServer.h"

#include"appframe.h"
AIAttack::AIAttack(AIComponent* owner)
	:AIState(owner)
	,_rotateAngle(0.f)
{

}

AIAttack::~AIAttack(){}

void AIAttack::OnEnter(){




}

void AIAttack::OnExist(){}

bool AIAttack::Process() {

	//�I�[�i�[�̑O���x�N�g��
	auto forward = _owner->GetOwner()->GetForward().Normalize();

	//�v���C���[�̈ʒu
	auto playerPos = _owner->GetOwner()->GetObjectServer()->GetPlayer()->GetPos();

	//�v���C���[�ƃI�[�i�[�̈ʒu�̍�
	auto playerToOwner = playerPos - _owner->GetOwner()->GetPos();

	//�v���C���[�ƃI�[�i�[�̈ʒu�̍��𐳋K��
	//�v���C���[����I�[�i�[�̌����׃N�g��
	playerToOwner.Normalized();
	
	//�I�[�i�[�������p�x
	_rotateAngle = Vector3::CrossAngleXZ(forward, playerToOwner);

	//��]�p�x�����ȉ��ɂȂ�����true��Ԃ��āA�������I��
	if(fabsf(_rotateAngle) < DegToRad(10.f)) { return true; }

	//�I�[�i�[�̊p�x
	auto ownerAngle = _owner->GetOwner()->GetEulerAngle();

	//��]�p�x��0���傫���ꍇ�A���ɉ�]
	if(_rotateAngle > 0.f) {
		ownerAngle.y -= DegToRad(3.f);
		_owner->GetOwner()->SetEulerAngle(ownerAngle);
	}
	//��]�p�x��0��菬�����ꍇ�A�E�ɉ�]
	else {
		ownerAngle.y += DegToRad(3.f);
		_owner->GetOwner()->SetEulerAngle(ownerAngle);
	}

	return true;
}
