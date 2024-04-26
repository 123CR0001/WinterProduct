#include"AIBackPatrol.h"
#include"AIComponent.h"
#include"Player.h"
#include"CommonSoldier.h"
#include"ObjectServer.h"
#include"appframe.h"
#include<algorithm>
#include"ModeGame.h"
#include"LightsOut.h"

AIBackPatrol::AIBackPatrol(AIComponent* owner)
	:AIState(owner)
	,_pointsNum(0)
{

}

AIBackPatrol::~AIBackPatrol(){}

void AIBackPatrol::OnEnter() {

	//BackPatrolGoal����Ȃ�Patrol�ɐ؂�ւ�
	if(_owner->GetPoints("BackPatrolGoal").empty()) { 
		_owner->ChangeState("Patrol"); 
		return; 
	}

	//���L�����̈ʒu
	const Vector3 pos = _owner->GetOwner()->GetPos();

	//�ړI�n
	Vector3 goal = _owner->GetPoints("BackPatrolGoal").front();

	//Navi�N���X���擾
	auto navi = _owner->GetOwner()->GetObjectServer()->GetNavi();

	//�ŒZ�o�H
	navi->FindPath(pos, goal, _owner->GetPoints(GetName()),50.f);
	
}
void AIBackPatrol::OnExist() {
	//�ڕW�n�_�̔ԍ���������
	_pointsNum = 0;
	//�ڕW�n�_���N���A
	_owner->GetPoints(GetName()).clear();
}

bool AIBackPatrol::Process() {

	//�v���C���[���������ɖڕW�n�_�ɂ����珄�񃋁[�g�ɖ߂�Patrol�ɐ؂�ւ�
	if (_owner->MoveTo(_owner->GetPoints(GetName()), _pointsNum)) {
		//�p�j�b�N��ԂɕύX
		_owner->ChangeState("Patrol");
		//�ڕW�n�_���N���A
		_owner->GetPoints("BackPatrolGoal").clear();
		return true;
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

	//��������������
	{
		Vector3 p;
		if(_owner->GetOwner()->GetObjectServer()->GetPhysWorld()->IsHear(_owner->GetOwner(), &p)) {
			_owner->AddPoint("MoveTo", p);
			_owner->ChangeState("Discovery");
			_owner->AddPoint("BackPatrolGoal", _owner->GetOwner()->GetPos());
		}
	}

	//LightsOut�ɂȂ�����,AIBlindWalk�ɕύX
	if(!_owner->GetOwner()->GetObjectServer()->GetGame()->GetLightsOut()->IsUse()) {
		_owner->ChangeState("BlindWalk");
	}


	return true;
}