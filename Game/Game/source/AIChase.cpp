#include"AIChase.h"
#include"AIComponent.h"
#include"Player.h"
#include"CommonSoldier.h"
#include"ObjectServer.h"
#include"appframe.h"
#include<algorithm>
#include"ModeGame.h"
#include"AIBackPatrol.h"

AIChase::AIChase(AIComponent* owner)
	:AIState(owner)
	, _pointsNum(0)
{
}

AIChase::~AIChase() {}

void AIChase::OnEnter() {

	GetShortestRoots();

}

void AIChase::OnExist() {
}

bool AIChase::Process() {


	//�v���C���[���������ɖڕW�n�_�ɂ����珄�񃋁[�g�ɖ߂�AIState�ɂ���
	if (_owner->MoveTo(_owner->GetPoints(GetName()), _pointsNum)) {
		if(!_owner->IsFound(_owner->GetChaseObject())) {
			_owner->ChangeState("LookAround");
		}
		else {
			_owner->ChangeState("Stare");
		}
	}

	//�I�u�W�F�N�g����������A�����X�V
	if (_owner->IsFound(_owner->GetChaseObject())) {
		auto object = _owner->GetChaseObject();
		//
		if (_owner->GetPoints(GetName()).size() > 0) {
			//���[�g�̍Ō�̍��W�Ƃ��ꂪ100�ȏゾ������A���[�g���X�V����
			if (Vector3D::LengthSquare(object->GetPos(), _owner->GetPoints(GetName()).back()) > 100 * 100) {
				GetShortestRoots();
			}
		}
	}

	//LightsOut�ɂȂ�����,AIBlindWalk�ɕύX
	if (ModeServer::GetInstance()->IsAdd("LightsOut")) {
		_owner->ChangeState("BlindWalk");
	}

	return true;
}

void AIChase::GetShortestRoots() {
	//�̂̃��[�g�͎̂Ă�
	_pointsNum = 0;
	_owner->GetPoints(GetName()).clear();

	//����AIState����������AIComponent����������ObjectBase����������Server
	auto navi = _owner->GetOwner()->GetObjectServer()->GetNavi();

	auto startPolygon = _owner->GetOwner()->GetPos();
	auto goalPolygon = _owner->GetChaseObject()->GetPos();

	//�ŒZ�o�H
	navi->BFS(startPolygon, goalPolygon, _owner->GetPoints(GetName()));
	
	
}