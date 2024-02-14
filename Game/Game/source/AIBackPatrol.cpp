#include"AIBackPatrol.h"
#include"AIChase.h"
#include"AIComponent.h"
#include"Player.h"
#include"CommonSoldier.h"
#include"ObjectServer.h"
#include"appframe.h"
#include<algorithm>
#include"ModeGame.h"

AIBackPatrol::AIBackPatrol(AIComponent* owner)
	:AIState(owner)
	,_pointsNum(0)
{

}

AIBackPatrol::~AIBackPatrol(){}

void AIBackPatrol::OnEnter() {
	//_pointsのサイズが0の場合は、もう巡回地点に戻ってきたと判断
	if (_owner->GetPoints(GetName()).size() == 0) {
		_owner->ChangeState("Patrol");
	}
}
void AIBackPatrol::OnExist() {
	if (_owner->GetPoints(GetName()).size() == 0) { return; }
	for (int a = 0; a < _owner->GetPoints(GetName()).size(); a++) {
		_owner->GetPoints(GetName()).erase(_owner->GetPoints(GetName()).begin());
	}
}

bool AIBackPatrol::Process() {

	//プレイヤーを見つけずに目標地点についたら巡回ルートに戻るPatrolに切り替え
	if (_owner->MoveTo(_owner->GetPoints(GetName()), _pointsNum)) {
		_owner->ChangeState("Patrol");
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
	if (ModeServer::GetInstance()->IsAdd("LightsOut")) {
		_owner->ChangeState("BlindWalk");
	}

	return true;
}