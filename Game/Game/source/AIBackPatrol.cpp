#include"AIBackPatrol.h"
#include"AIComponent.h"
#include"Player.h"
#include"CommonSoldier.h"
#include"ObjectServer.h"
#include"appframe.h"
#include<algorithm>
#include"ModeGame.h"
#include"LightsOut.h"

AIBackPatrol::AIBackPatrol(AIComponent* owner)
	:AIState(owner)
	,_pointsNum(0)
{

}

AIBackPatrol::~AIBackPatrol(){}

void AIBackPatrol::OnEnter() {

	if(_owner->GetPoints("BackPatrolGoal").empty()) { 
		_owner->ChangeState("Patrol"); 
		return; 
	}

	//自キャラの位置
	const Vector3 pos = _owner->GetOwner()->GetPos();

	//目的地
	Vector3 goal = _owner->GetPoints("BackPatrolGoal").front();

	auto navi = _owner->GetOwner()->GetObjectServer()->GetNavi();

	//最短経路
	navi->BFS(pos, goal, _owner->GetPoints(GetName()));
	
}
void AIBackPatrol::OnExist() {
	_pointsNum = 0;
	_owner->GetPoints(GetName()).clear();
}

bool AIBackPatrol::Process() {

	//プレイヤーを見つけずに目標地点についたら巡回ルートに戻るPatrolに切り替え
	if (_owner->MoveTo(_owner->GetPoints(GetName()), _pointsNum)) {
		_owner->ChangeState("Patrol");
		_owner->GetPoints("BackPatrolGoal").clear();
		return true;
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
					_owner->AddPoint("MoveTo", objects[a]->GetPos());
					_owner->ChangeState("Discovery");
					break;
				}
			}
		}
	}

	//音が聞こえたか
	{
		Vector3 p;
		if(_owner->GetOwner()->GetObjectServer()->GetPhysWorld()->IsHear(_owner->GetOwner(), &p)) {
			_owner->AddPoint("MoveTo", p);
			_owner->ChangeState("Discovery");
			_owner->AddPoint("BackPatrolGoal", _owner->GetOwner()->GetPos());
		}
	}
	//LightsOutになったら,AIBlindWalkに変更
	if(!_owner->GetOwner()->GetObjectServer()->GetGame()->GetLightsOut()->IsUse()) {
		_owner->ChangeState("BlindWalk");
	}


	return true;
}