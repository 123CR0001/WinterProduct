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
	,_patrolPointsNum(1)
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
		int oldNum = _patrolPointsNum;
		_owner->MoveTo(_owner->GetPoints(GetName()), _patrolPointsNum);

		//�|�C���g�ɓ���
		if (oldNum != _patrolPointsNum) {
			_owner->ChangeState("LookAround");
		}
	}

	//AIState��ύX������
	bool isChangeState = false;

	//���������������H
	{
		Vector3D p;
		if (_owner->GetOwner()->GetObjectServer()->GetPhysWorld()->IsHear(_owner->GetOwner(), &p)) {
			isChangeState = true;
			_owner->DeletePoint("CheckPoint");
			_owner->AddPoint("CheckPoint", p);
			_owner->ChangeState("Discovery");
			_owner->AddPoint("BackPatrolGoal", _owner->GetOwner()->GetPos());
		}
	}

	//�o�^���Ă��閼�O�Ɠ������O�����I�u�W�F�N�g�����E�ɓ��ꂽ��
	
		auto objects = _owner->GetOwner()->GetObjectServer()->GetObjects();
		std::vector<std::string>names;

		names.emplace_back("player");
		names.emplace_back("Decoy");

		for (int a = 0; a < objects.size(); a++) {

			for (int b = 0; b < names.size(); b++) {

				//�o�^�������O��
				if (objects[a]->GetName() != names[b]) { continue; }

				//���E�ɓ����Ă��邩
				if (_owner->IsFound(objects[a])) { 

					isChangeState = true;
					//�ǂ�������I�u�W�F�N�g�̃A�h���X��o�^
					_owner->SetChaseObject(objects[a]);
					//AIState��ύX
					_owner->AddPoint("CheckPoint", objects[a]->GetPos());
					_owner->ChangeState("Discovery");
					_owner->AddPoint("BackPatrolGoal", _owner->GetOwner()->GetPos());
					return true;
				
				}
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