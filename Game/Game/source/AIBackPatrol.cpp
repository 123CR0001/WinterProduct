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

	if(_owner->GetPoints("BackPatrolGoal").empty()) { 
		_owner->ChangeState("Patrol"); 
		return; 
	}

	//���L�����̈ʒu
	const Vector3 pos = _owner->GetOwner()->GetPos();

	//�ړI�n
	Vector3 goal = _owner->GetPoints("BackPatrolGoal").front();

	auto navi = _owner->GetOwner()->GetObjectServer()->GetNavi();

	//�ŒZ�o�H
	navi->BFS(pos, goal, _owner->GetPoints(GetName()));
	
}
void AIBackPatrol::OnExist() {
	_pointsNum = 0;
	_owner->GetPoints(GetName()).clear();
}

bool AIBackPatrol::Process() {

	//�v���C���[���������ɖڕW�n�_�ɂ����珄�񃋁[�g�ɖ߂�Patrol�ɐ؂�ւ�
	if (_owner->MoveTo(_owner->GetPoints(GetName()), _pointsNum)) {
		_owner->ChangeState("Patrol");
		_owner->GetPoints("BackPatrolGoal").clear();
		return true;
	}

	//�o�^���Ă��閼�O�Ɠ������O�����I�u�W�F�N�g�����E�ɓ��ꂽ��
	{
		auto objects = _owner->GetOwner()->GetObjectServer()->GetObjects();
		std::vector<std::string>names;

		names.emplace_back("player");
		names.emplace_back("Decoy");

		for (int a = 0; a < objects.size(); a++) {

			//���E�ɓ����Ă��Ȃ�
			if (!_owner->IsFound(objects[a])) { continue; }

			for (int b = 0; b < names.size(); b++) {
				if (objects[a]->GetName() == names[b]) {
					//�ǂ�������I�u�W�F�N�g�̃A�h���X��o�^
					_owner->SetChaseObject(objects[a]);
					//AIState��ύX
					_owner->AddPoint("MoveTo", objects[a]->GetPos());
					_owner->ChangeState("Discovery");
					break;
				}
			}
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