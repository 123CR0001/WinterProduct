#include"AIPatrol.h"
#include"AIComponent.h"
#include"ObjectBase.h"
#include"CommonSoldier.h"
#include"ObjectServer.h"
#include<algorithm>
#include"AIBackPatrol.h"
#include"PhysWorld.h"
#include"Player.h"

AIPatrol::AIPatrol(AIComponent* owner)
	:AIState(owner)
	,_patrolPointsNum(1)
{}

AIPatrol::~AIPatrol(){}


void AIPatrol::OnEnter() {
}

void AIPatrol::OnExist() {

}

bool AIPatrol::Process() {

	auto owner = _owner->GetOwner();
	auto objectServer = _owner->GetOwner()->GetObjectServer();

	//�o�H������
	_owner->MoveTo(_owner->GetPoints(GetName()), _patrolPointsNum);

	//���������������H
	{
		Vector3D p;
		if (_owner->GetOwner()->GetObjectServer()->GetPhysWorld()->IsHear(_owner->GetOwner(), &p)) {
			_owner->AddPoint("CheckPoint", p);
			_owner->ChangeState("CheckPoint");
		}
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
					_owner->ChangeState("Chase");
					break;
				}
			}
		}
	}

	//LightsOut�ɂȂ�����,AIBlindWalk�ɕύX
	if (ModeServer::GetInstance()->IsAdd("LightsOut")) {
		_owner->ChangeState("BlindWalk");
	}
	
	return true;
}