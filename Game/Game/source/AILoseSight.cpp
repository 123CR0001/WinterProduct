#include"AILoseSight.h"
#include"AIComponent.h"
#include"ObjectBase.h"

#include"ObjectServer.h"
#include"PhysWorld.h"

#include"ModeGame.h"

AILoseSight::AILoseSight(AIComponent* owner)
	:AIState(owner)
	, _frameCnt(0)
{

}

AILoseSight::~AILoseSight() {

}

void AILoseSight::OnEnter() {
	//LoseSgihtの時間を設定する
	_frameCnt = 120;
}

void AILoseSight::OnExist() {

}

bool AILoseSight::Process() {
	_frameCnt--;

	//一定時間経過したらBackPatrolにステートを変更する
	if (_frameCnt <= 0) {
		_owner->ChangeState("BackPatrol");
	}

	//BackPatrolは最初に登録された座標に戻るので、Patrol → LookAround → Chase(もしくはCheckPoint) → LookAround → BackPatrolでも、Patrolで記録された座標に戻る

	//音が聞こえたか？
	{
		Vector3 p;
		if (_owner->GetOwner()->GetObjectServer()->GetPhysWorld()->IsHear(_owner->GetOwner(), &p)) {
			_owner->AddPoint("MoveTo", p);
			_owner->ChangeState("Discovery");
			_owner->AddPoint("BackPatrolGoal", _owner->GetOwner()->GetPos());
		}
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

	//LightsOutになったら,AIBlindWalkに変更
	if (_owner->GetOwner()->GetObjectServer()->GetGame()->IsUsingLightsOut()) {
		_owner->ChangeState("BlindWalk");
	}

	return true;
}