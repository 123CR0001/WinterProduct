#include"AIPatrol.h"
#include"AIComponent.h"
#include"ObjectBase.h"
#include"CommonSoldier.h"
#include"ObjectServer.h"
#include<algorithm>
#include"AIBackPatrol.h"
#include"PhysWorld.h"
#include"Player.h"

#include"ModeGame.h"
#include"LightsOut.h"

AIPatrol::AIPatrol(AIComponent* owner)
	:AIState(owner)
	,_pointsNum(1)
{}

AIPatrol::~AIPatrol(){}


void AIPatrol::OnEnter() {
	//���񂷂�o�H���Ȃ��̂ŁAAIStay�ɐ؂�ւ���
	if(_owner->GetPoints(GetName()).size() == 1) { _owner->ChangeState("Stay"); }
}

void AIPatrol::OnExist() {

}

bool AIPatrol::Process() {

	auto owner = _owner->GetOwner();
	auto objectServer = _owner->GetOwner()->GetObjectServer();

	//�o�H������
	{
		//�ڕW���W�ɕω����Ȃ���
		int oldNum = _pointsNum;
		_owner->MoveTo(_owner->GetPoints(GetName()), _pointsNum);

		//�|�C���g�ɓ���
		if (oldNum != _pointsNum) {
			_owner->ChangeState("LookAround");
		}
	}

	//AIState��ύX������
	bool isChangeState = false;

	//���������������H
	{
		Vector3 p;
		if (_owner->GetOwner()->GetObjectServer()->GetPhysWorld()->IsHear(_owner->GetOwner(), &p)) {
			isChangeState = true;

			//
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
			_owner->SetChaseObject(object);
			_owner->AddPoint("MoveTo", object->GetPos());
			_owner->ChangeState("Discovery");
			_owner->AddPoint("BackPatrolGoal", _owner->GetOwner()->GetPos());
		}
	}
	

	//LightsOut�ɂȂ�����,AIBlindWalk�ɕύX
	if(!_owner->GetOwner()->GetObjectServer()->GetGame()->GetLightsOut()->IsUse()) {
		_owner->ChangeState("BlindWalk");
	}


	//AIState���ύX����Ă�����A�ړI�n�̍��W��AIBackPatrol�̍��W�R���e�i�ɓo�^
	//���̓o�^�������W����ɁA���̈ʒu�ɖ߂��Ă���

	
	return true;
}