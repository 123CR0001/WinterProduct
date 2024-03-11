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

	//一番最後に追加された座標へ向かう
	const Vector3D checkPoint = _owner->GetPoints(GetName()).back();
	//昔のルートは捨てる
	_pointsNum = 0;
	_owner->GetPoints(GetName()).clear();

	//このAIStateを所持するAIComponentを所持するObjectBaseが所属するServer
	auto server = _owner->GetOwner()->GetObjectServer();
	auto owner = _owner->GetOwner();

	auto navi = _owner->GetOwner()->GetObjectServer()->GetNavi();

	auto startPolygon = _owner->GetOwner()->GetPos();
	auto goalPolygon = checkPoint;

	//最短経路
		navi->BFS(startPolygon,goalPolygon,_owner->GetPoints(GetName()));
	

}

void AICheckPoint::OnExist() {
	_owner->GetPoints(GetName()).clear();
}

bool AICheckPoint::Process() {

	//移動　最後の座標まで到達したら、巡回経路に戻る
	if (_owner->MoveTo(_owner->GetPoints(GetName()), _pointsNum)) {
		_owner->ChangeState("LookAround");
	}

	//登録している名前と同じ名前を持つオブジェクトを視界に入れたか
	{
		auto objects = _owner->GetOwner()->GetObjectServer()->GetObjects();
		std::vector<std::string>names;

		names.emplace_back("player");
		names.emplace_back("Decoy");

		for (int a = 0; a < objects.size(); a++) {

			//視界に入っていない
			if (!_owner->IsFound(objects[a])) { continue; }

			for (int b = 0; b < names.size(); b++) {
				if (objects[a]->GetName() == names[b]) {
					//追いかけるオブジェクトのアドレスを登録
					_owner->SetChaseObject(objects[a]);
					//AIStateを変更
					_owner->ChangeState("Chase");
					break;
				}
			}
		}
	}


	//LightsOutになったら,AIBlindWalkに変更
	if(!_owner->GetOwner()->GetObjectServer()->GetGame()->GetLightsOut()->IsUse()) {
		_owner->ChangeState("BlindWalk");
	}


	return true;
}