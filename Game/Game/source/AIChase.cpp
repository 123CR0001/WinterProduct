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


	//プレイヤーを見つけずに目標地点についたら巡回ルートに戻るAIStateにする
	if (_owner->MoveTo(_owner->GetPoints(GetName()), _pointsNum)) {
		if(!_owner->IsFound(_owner->GetChaseObject())) {
			_owner->ChangeState("LookAround");
		}
		else {
			_owner->ChangeState("Stare");
		}
	}

	//オブジェクトを見つけたら、随時更新
	if (_owner->IsFound(_owner->GetChaseObject())) {
		auto object = _owner->GetChaseObject();
		//
		if (_owner->GetPoints(GetName()).size() > 0) {
			//ルートの最後の座標とずれが100以上だったら、ルートを更新する
			if (Vector3D::LengthSquare(object->GetPos(), _owner->GetPoints(GetName()).back()) > 100 * 100) {
				GetShortestRoots();
			}
		}
	}

	//LightsOutになったら,AIBlindWalkに変更
	if (ModeServer::GetInstance()->IsAdd("LightsOut")) {
		_owner->ChangeState("BlindWalk");
	}

	return true;
}

void AIChase::GetShortestRoots() {
	//昔のルートは捨てる
	_pointsNum = 0;
	_owner->GetPoints(GetName()).clear();

	//このAIStateを所持するAIComponentを所持するObjectBaseが所属するServer
	auto navi = _owner->GetOwner()->GetObjectServer()->GetNavi();

	auto startPolygon = _owner->GetOwner()->GetPos();
	auto goalPolygon = _owner->GetChaseObject()->GetPos();

	//最短経路
	navi->BFS(startPolygon, goalPolygon, _owner->GetPoints(GetName()));
	
	
}