#include"AIBackPatrol.h"
#include"AIChasePlayer.h"
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
	//プレイヤーを見つけたら、CheckPlayerに切り替え
	if (_owner->GetOwner()->GetObjectServer()->GetPlayer()) {
		_owner->ChangeState("ChasePlayer");
	}

	//LightsOutになったら,AIBlindWalkに変更
	if (ModeServer::GetInstance()->IsAdd("LightsOut")) {
		_owner->ChangeState("BlindWalk");
	}

	return true;
}