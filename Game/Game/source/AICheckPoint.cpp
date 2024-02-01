#include"AICheckPoint.h"
#include"AIComponent.h"
#include"ObjectServer.h"
#include"CommonSoldier.h"

AICheckPoint::AICheckPoint(AIComponent* owner)
	:AIState(owner)
	,_pointsNum(0)
{

}

AICheckPoint::~AICheckPoint(){}

void AICheckPoint::OnEnter() {
	if (_owner->GetPoints(GetName()).size() < 0) {
		//state変更
	}
	else {
		const Vector3D checkPoint = _owner->GetPoints(GetName()).front();
		//昔のルートは捨てる
		_pointsNum = 0;
		_owner->GetPoints(GetName()).clear();

		//このAIStateを所持するAIComponentを所持するObjectBaseが所属するServer
		auto server = _owner->GetOwner()->GetObjectServer();

		auto iter = std::find(
			server->GetCommonSoldiers().begin(),
			server->GetCommonSoldiers().end(),
			_owner->GetOwner()
		);

		//されていたら、処理をする
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
					//経路探索失敗　state 変更

					int i = 0;
				}
			}
		}
	}
}

void AICheckPoint::OnExist() {
	_owner->GetPoints(GetName()).clear();
}

bool AICheckPoint::Process() {
	//Serverの_enemysに登録されているか
	auto iter = std::find(_owner->GetOwner()->GetObjectServer()->GetCommonSoldiers().begin(),
		_owner->GetOwner()->GetObjectServer()->GetCommonSoldiers().end(),
		_owner->GetOwner()
	);

	//されていたら、処理をする
	if (iter != _owner->GetOwner()->GetObjectServer()->GetCommonSoldiers().end()) {
		if ((*iter)->MoveRoute(_owner->GetPoints(GetName()), _pointsNum)) {
			_owner->ChangeState("BackPatrol");
		}
		if ((*iter)->IsPlayerFound()) {
			_owner->ChangeState("ChasePlayer");
		}
	}
	return true;
}