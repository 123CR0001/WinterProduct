#include"AICheckPoint.h"
#include"AIComponent.h"
#include"ObjectServer.h"
#include"CommonSoldier.h"
#include"Player.h"

AICheckPoint::AICheckPoint(AIComponent* owner)
	:AIState(owner)
	,_pointsNum(0)
	,_frameCnt(0)
{

}

AICheckPoint::~AICheckPoint(){}

void AICheckPoint::OnEnter() {

	const Vector3D checkPoint = _owner->GetPoints(GetName()).front();
	//�̂̃��[�g�͎̂Ă�
	_pointsNum = 0;
	_owner->GetPoints(GetName()).clear();

	//����AIState����������AIComponent����������ObjectBase����������Server
	auto server = _owner->GetOwner()->GetObjectServer();
	auto owner = _owner->GetOwner();

	VECTOR center = MV1CollCheck_Line(
		server->GetNavigationHandle(),
		server->GetNavigationAttachIndex(),
		DxConverter::VecToDx(owner->GetPos() - Vector3D(0.f, 100.f, 0.f)),
		DxConverter::VecToDx(owner->GetPos() + Vector3D(0.f, 100.f, 0.f))
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
	auto checkPointOnPolygon = Navi::GetHitPoygon(checkPoint, hitPolygons);

	if (ownerOnPolygon && checkPointOnPolygon) {

		Navi::BFS(conectPolygonMap,
			ownerOnPolygon,
			checkPointOnPolygon,
			_owner->GetPoints(GetName())
		);

	}

}

void AICheckPoint::OnExist() {
	_owner->GetPoints(GetName()).clear();
	_frameCnt = 0;
}

bool AICheckPoint::Process() {

	//�ړ��@�Ō�̍��W�܂œ��B������A����o�H�ɖ߂�
	if (_owner->MoveTo(_owner->GetPoints(GetName()), _pointsNum)) {
		_owner->ChangeState("BackPatrol");
	}

	//�v���C���[����������
	if (_owner->IsFound(_owner->GetOwner()->GetObjectServer()->GetPlayer())) {
		_owner->ChangeState("ChasePlayer");
	}

	_frameCnt++;
	if (_frameCnt == 30) {
		if (_owner->GetPoints("BackPatrol").size() == 0) {
			_owner->AddPoint("BackPatrol", _owner->GetOwner()->GetPos());
		}
		else {
			_owner->InsertPoint("BackPatrol", _owner->GetOwner()->GetPos(), 0);
		}
		_frameCnt = 0;
	}

	//LightsOut�ɂȂ�����,AIBlindWalk�ɕύX
	if (ModeServer::GetInstance()->IsAdd("LightsOut")) {
		_owner->ChangeState("BlindWalk");
	}

	return true;
}