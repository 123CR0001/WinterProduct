#include"AIBackPatrol.h"
#include"AIChase.h"
#include"AIComponent.h"
#include"Player.h"
#include"CommonSoldier.h"
#include"ObjectServer.h"
#include"appframe.h"
#include<algorithm>
#include"ModeGame.h"

AIBackPatrol::AIBackPatrol(AIComponent* owner)
	:AIState(owner)
	,_pointsNum(0)
{

}

AIBackPatrol::~AIBackPatrol(){}

void AIBackPatrol::OnEnter() {
	auto navi = _owner->GetOwner()->GetObjectServer()->GetNavi();

	auto startPolygon = navi->GetHitPoygon(_owner->GetOwner()->GetPos());
	auto goalPolygon = navi->GetHitPoygon(_owner->GetPoints("Patrol").front());

	//�ŒZ�o�H
	if(startPolygon && goalPolygon) {
		navi->BFS(startPolygon, goalPolygon, _owner->GetPoints(GetName()));
	}
}
void AIBackPatrol::OnExist() {
	if (_owner->GetPoints(GetName()).size() == 0) { return; }
	for (int a = 0; a < _owner->GetPoints(GetName()).size(); a++) {
		_owner->GetPoints(GetName()).erase(_owner->GetPoints(GetName()).begin());
	}
}

bool AIBackPatrol::Process() {

	//�v���C���[���������ɖڕW�n�_�ɂ����珄�񃋁[�g�ɖ߂�Patrol�ɐ؂�ւ�
	if (_owner->MoveTo(_owner->GetPoints(GetName()), _pointsNum)) {
		_owner->ChangeState("Patrol");
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