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

	//BackPatrolGoalが空ならPatrolに切り替え
	if(_owner->GetPoints("BackPatrolGoal").empty()) { 
		_owner->ChangeState("Patrol"); 
		return; 
	}

	//自キャラの位置
	const Vector3 pos = _owner->GetOwner()->GetPos();

	//目的地
	Vector3 goal = _owner->GetPoints("BackPatrolGoal").front();

	//Naviクラスを取得
	auto navi = _owner->GetOwner()->GetObjectServer()->GetNavi();

	//最短経路
	navi->FindPath(pos, goal, _owner->GetPoints(GetName()),50.f);
	
}
void AIBackPatrol::OnExist() {
	//目標地点の番号を初期化
	_pointsNum = 0;
	//目標地点をクリア
	_owner->GetPoints(GetName()).clear();
}

bool AIBackPatrol::Process() {

	//プレイヤーを見つけずに目標地点についたら巡回ルートに戻るPatrolに切り替え
	if (_owner->MoveTo(_owner->GetPoints(GetName()), _pointsNum)) {
		//パニック状態に変更
		_owner->ChangeState("Patrol");
		//目標地点をクリア
		_owner->GetPoints("BackPatrolGoal").clear();
		return true;
	}

	//登録している名前と同じ名前を持つオブジェクトを視界に入れたか
	{
		auto object = _owner->IsFound();

		if (object) {
			//AIStateを変更
			_owner->AddPoint("MoveTo", object->GetPos());
			_owner->ChangeState("Discovery");
			_owner->AddPoint("BackPatrolGoal", _owner->GetOwner()->GetPos());
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