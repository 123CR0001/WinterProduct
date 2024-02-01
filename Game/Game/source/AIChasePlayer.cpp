#include"AIChasePlayer.h"
#include"AIComponent.h"
#include"Player.h"
#include"CommonSoldier.h"
#include"ObjectServer.h"
#include"appframe.h"
#include<algorithm>
#include"ModeGame.h"
#include"AIBackPatrol.h"

AIChasePlayer::AIChasePlayer(AIComponent* owner,AIBackPatrol* AI)
	:AIState(owner)
	,_AIBackPatrol(AI)
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

	//Serverの_enemysに登録されているか
	auto iter = std::find(_owner->GetOwner()->GetObjectServer()->GetCommonSoldiers().begin(),
		_owner->GetOwner()->GetObjectServer()->GetCommonSoldiers().end(),
		_owner->GetOwner()
	);

	//されていたら、処理をする
	if (iter != _owner->GetOwner()->GetObjectServer()->GetCommonSoldiers().end()) {

		//プレイヤーを見つけずに目標地点についたら巡回ルートに戻るAIStateにする
		if ((*iter)->MoveRoute(_points, _pointsNum)) {
			_owner->ChangeState("BackPatrol");
		}

		//プレイヤーを見つけたら、随時更新
		if ((*iter)->IsPlayerFound()) {
			auto player = _owner->GetOwner()->GetObjectServer()->GetPlayer();
			//
			if (_points.size() > 0) {
				//
				if (Vector3D::LengthSquare(player->GetPos(), _points.back()) > 10000) {
					GetShortestRoots();
				}
			}
		}

	}

	_frameCnt++;
	if (_frameCnt % 30 == 0) {
		_owner->AddPoint("BackPatrol", _owner->GetOwner()->GetPos());
		_frameCnt = 0;
	}
	return true;
}

bool AIChasePlayer::GetShortestRoots() {
	//昔のルートは捨てる
	_pointsNum = 0;
	_points.clear();

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
		auto playerOnPolygon = Navi::GetHitPoygon(_owner->GetOwner()->GetObjectServer()->GetPlayer()->GetPos(), hitPolygons);

		if (ownerOnPolygon && playerOnPolygon) {
			if (Navi::BFS(conectPolygonMap,
				ownerOnPolygon,
				playerOnPolygon,
				_points
			)) {
				return true;
			}
		}
	}

	return false;
}