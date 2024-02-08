#include"AIChasePlayer.h"
#include"AIComponent.h"
#include"Player.h"
#include"CommonSoldier.h"
#include"ObjectServer.h"
#include"appframe.h"
#include<algorithm>
#include"ModeGame.h"
#include"AIBackPatrol.h"

AIChasePlayer::AIChasePlayer(AIComponent* owner)
	:AIState(owner)
	, _pointsNum(0)
	,_frameCnt(0)
{
}

AIChasePlayer::~AIChasePlayer() {}

void AIChasePlayer::OnEnter() {

	_frameCnt = 0;

	if (GetShortestRoots()) { return; }

	_owner->ChangeState("BackPatrol");
}

void AIChasePlayer::OnExist() {

}

bool AIChasePlayer::Process() {

	//Server��_enemys�ɓo�^����Ă��邩
	auto iter = std::find(_owner->GetOwner()->GetObjectServer()->GetCommonSoldiers().begin(),
		_owner->GetOwner()->GetObjectServer()->GetCommonSoldiers().end(),
		_owner->GetOwner()
	);

	//����Ă�����A����������
	if (iter != _owner->GetOwner()->GetObjectServer()->GetCommonSoldiers().end()) {

		//�v���C���[���������ɖڕW�n�_�ɂ����珄�񃋁[�g�ɖ߂�AIState�ɂ���
		if ((*iter)->MoveRoute(_owner->GetPoints(GetName()), _pointsNum)) {
			_owner->ChangeState("BackPatrol");
		}

		//�v���C���[����������A�����X�V
		if ((*iter)->IsPlayerFound()) {
			auto player = _owner->GetOwner()->GetObjectServer()->GetPlayer();
			//
			if (_owner->GetPoints(GetName()).size() > 0) {
				//���[�g�̍Ō�̍��W�Ƃ��ꂪ100�ȏゾ������A���[�g���X�V����
				if (Vector3D::LengthSquare(player->GetPos(), _owner->GetPoints(GetName()).back()) > 10000) {
					GetShortestRoots();
				}
			}
		}

	}

	_frameCnt++;
	if (_frameCnt == 30) {
		if (_owner->GetPoints("BackPatrol").size() == 0) {
			_owner->AddPoint("BackPatrol", _owner->GetOwner()->GetPos());
		}
		else {
			_owner->InsertPoint("BackPatrol", _owner->GetOwner()->GetPos(),0);
		}
		_frameCnt = 0;
	}

	//LightsOut�ɂȂ�����,AIBlindWalk�ɕύX
	if (ModeServer::GetInstance()->IsAdd("LightsOut")) {
		_owner->ChangeState("BlindWalk");
	}

	return true;
}

bool AIChasePlayer::GetShortestRoots() {
	//�̂̃��[�g�͎̂Ă�
	_pointsNum = 0;
	_owner->GetPoints(GetName()).clear();

	//����AIState����������AIComponent����������ObjectBase����������Server
	auto server = _owner->GetOwner()->GetObjectServer();

	auto iter = std::find(
		server->GetCommonSoldiers().begin(),
		server->GetCommonSoldiers().end(),
		_owner->GetOwner()
	);

	//����Ă�����A����������
	if (iter != _owner->GetOwner()->GetObjectServer()->GetCommonSoldiers().end()) {

		VECTOR center = MV1CollCheck_Line(
			server->GetNavigationHandle(),
			server->GetNavigationAttachIndex(),
			DxConverter::VecToDx((*iter)->GetPos() - Vector3D(0.f, 100.f, 0.f)),
			DxConverter::VecToDx((*iter)->GetPos() + Vector3D(0.f, 100.f, 0.f))
		).HitPosition;

		std::vector<Polygon3D>hitPolygons;

		MV1_COLL_RESULT_POLY_DIM result = MV1CollCheck_Sphere(
			server->GetNavigationHandle(),
			server->GetNavigationAttachIndex(),
			center,
			(*iter)->GetVisionDist()
		);

		for (int a = 0; a < result.HitNum; a++) {
			Polygon3D add(
				DxConverter::DxToVec(result.Dim[a].Position[0]),
				DxConverter::DxToVec(result.Dim[a].Position[1]),
				DxConverter::DxToVec(result.Dim[a].Position[2])
			);
			hitPolygons.emplace_back(add);
		}
		MV1CollResultPolyDimTerminate(result);

		ConectPolygonMap conectPolygonMap;
		Navi::GetConectPolygonMap(hitPolygons, conectPolygonMap);

		auto ownerOnPolygon = Navi::GetHitPoygon(_owner->GetOwner()->GetPos(), hitPolygons);
		auto playerOnPolygon = Navi::GetHitPoygon(_owner->GetOwner()->GetObjectServer()->GetPlayer()->GetPos(), hitPolygons);

		if (ownerOnPolygon && playerOnPolygon) {
			if (Navi::BFS(conectPolygonMap,
				ownerOnPolygon,
				playerOnPolygon,
				_owner->GetPoints(GetName())
			)) {
				return true;
			}
		}
	}

	return false;
}