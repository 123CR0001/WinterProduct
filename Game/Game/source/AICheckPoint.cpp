#include"AICheckPoint.h"
#include"AIComponent.h"
#include"ObjectServer.h"
#include"appframe.h"
#include"CommonSoldier.h"
#include"Player.h"

#include"ModeGame.h"
#include"LightsOut.h"

AICheckPoint::AICheckPoint(AIComponent* owner)
	:AIState(owner)
	,_pointsNum(0)
{

}

AICheckPoint::~AICheckPoint(){}

void AICheckPoint::OnEnter() {

	//��ԍŌ�ɒǉ����ꂽ���W�֌�����
	const Vector3D checkPoint = _owner->GetPoints(GetName()).back();
	//�̂̃��[�g�͎̂Ă�
	_pointsNum = 0;
	_owner->GetPoints(GetName()).clear();

	//����AIState����������AIComponent����������ObjectBase����������Server
	auto server = _owner->GetOwner()->GetObjectServer();
	auto owner = _owner->GetOwner();

	auto navi = _owner->GetOwner()->GetObjectServer()->GetNavi();

	auto startPolygon = _owner->GetOwner()->GetPos();
	auto goalPolygon = checkPoint;

	//�ŒZ�o�H
		navi->BFS(startPolygon,goalPolygon,_owner->GetPoints(GetName()));
	

}

void AICheckPoint::OnExist() {
	_owner->GetPoints(GetName()).clear();
}

bool AICheckPoint::Process() {

	//�ړ��@�Ō�̍��W�܂œ��B������A����o�H�ɖ߂�
	if (_owner->MoveTo(_owner->GetPoints(GetName()), _pointsNum)) {
		_owner->ChangeState("LookAround");
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
	if(!_owner->GetOwner()->GetObjectServer()->GetGame()->GetLightsOut()->IsUse()) {
		_owner->ChangeState("BlindWalk");
	}


	return true;
}