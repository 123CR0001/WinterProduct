#include"AICheckPoint.h"
#include"AIComponent.h"
#include"ObjectServer.h"
#include"CommonSoldier.h"

AICheckPoint::AICheckPoint(AIComponent* owner)
	:AIState(owner)
	,_pointsNum(0)
	,_frameCnt(0)
{

}

AICheckPoint::~AICheckPoint(){}

void AICheckPoint::OnEnter() {

	const Vector3D checkPoint = _owner->GetPoints(GetName()).front();
	//Ì‚Ìƒ‹[ƒg‚ÍŽÌ‚Ä‚é
	_pointsNum = 0;
	_owner->GetPoints(GetName()).clear();

	//‚±‚ÌAIState‚ðŠŽ‚·‚éAIComponent‚ðŠŽ‚·‚éObjectBase‚ªŠ‘®‚·‚éServer
	auto server = _owner->GetOwner()->GetObjectServer();

	auto iter = std::find(
		server->GetCommonSoldiers().begin(),
		server->GetCommonSoldiers().end(),
		_owner->GetOwner()
	);

	//‚³‚ê‚Ä‚¢‚½‚çAˆ—‚ð‚·‚é
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
		auto checkPointOnPolygon = Navi::GetHitPoygon(checkPoint, hitPolygons);

		if (ownerOnPolygon && checkPointOnPolygon) {
			if (!Navi::BFS(conectPolygonMap,
				ownerOnPolygon,
				checkPointOnPolygon,
				_owner->GetPoints(GetName())
			)) {
				//Œo˜H’TõŽ¸”s@state •ÏX

			}
		}
	}
	
}

void AICheckPoint::OnExist() {
	_owner->GetPoints(GetName()).clear();
	_frameCnt = 0;
}

bool AICheckPoint::Process() {
	//Server‚Ì_enemys‚É“o˜^‚³‚ê‚Ä‚¢‚é‚©
	auto iter = std::find(_owner->GetOwner()->GetObjectServer()->GetCommonSoldiers().begin(),
		_owner->GetOwner()->GetObjectServer()->GetCommonSoldiers().end(),
		_owner->GetOwner()
	);

	//‚³‚ê‚Ä‚¢‚½‚çAˆ—‚ð‚·‚é
	if (iter != _owner->GetOwner()->GetObjectServer()->GetCommonSoldiers().end()) {
		if ((*iter)->MoveRoute(_owner->GetPoints(GetName()), _pointsNum)) {
			_owner->ChangeState("BackPatrol");
		}
		if ((*iter)->IsPlayerFound()) {
			_owner->ChangeState("ChasePlayer");
		}
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
	return true;
}