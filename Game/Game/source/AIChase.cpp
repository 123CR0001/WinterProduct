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
	,_frameCnt(0)
{
}

AIChase::~AIChase() {}

void AIChase::OnEnter() {

	_frameCnt = 0;

	GetShortestRoots();

	if(_owner->GetPoints(GetName()).size() == 0)_owner->ChangeState("BackPatrol");
}

void AIChase::OnExist() {
}

bool AIChase::Process() {


	//�v���C���[���������ɖڕW�n�_�ɂ����珄�񃋁[�g�ɖ߂�AIState�ɂ���
	if (_owner->MoveTo(_owner->GetPoints(GetName()), _pointsNum)) {
		_owner->ChangeState("LookAround");
	}

	//�I�u�W�F�N�g����������A�����X�V
	if (_owner->IsFound(_owner->GetChaseObject())) {
		auto object = _owner->GetChaseObject();
		//
		if (_owner->GetPoints(GetName()).size() > 0) {
			//���[�g�̍Ō�̍��W�Ƃ��ꂪ100�ȏゾ������A���[�g���X�V����
			if (Vector3D::LengthSquare(object->GetPos(), _owner->GetPoints(GetName()).back()) > 10000) {
				GetShortestRoots();
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

void AIChase::GetShortestRoots() {
	//�̂̃��[�g�͎̂Ă�
	_pointsNum = 0;
	_owner->GetPoints(GetName()).clear();

	//����AIState����������AIComponent����������ObjectBase����������Server
	auto server = _owner->GetOwner()->GetObjectServer();

	VECTOR center = MV1CollCheck_Line(
		server->GetNavigationHandle(),
		server->GetNavigationAttachIndex(),
		DxConverter::VecToDx(_owner->GetOwner()->GetPos() - Vector3D(0.f, 100.f, 0.f)),
		DxConverter::VecToDx(_owner->GetOwner()->GetPos() + Vector3D(0.f, 100.f, 0.f))
	).HitPosition;

	std::vector<Polygon3D>hitPolygons;

	MV1_COLL_RESULT_POLY_DIM result = MV1CollCheck_Sphere(
		server->GetNavigationHandle(),
		server->GetNavigationAttachIndex(),
		center,
		_owner->GetViewDist()
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
	auto objectOnPolygon = Navi::GetHitPoygon(_owner->GetChaseObject()->GetPos(), hitPolygons);

	if (ownerOnPolygon && objectOnPolygon) {
		Navi::BFS(conectPolygonMap,
			ownerOnPolygon,
			objectOnPolygon,
			_owner->GetPoints(GetName()
			));
	}
	
}