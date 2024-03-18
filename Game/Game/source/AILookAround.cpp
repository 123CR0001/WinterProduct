#include"AILookAround.h"
#include"AIComponent.h"
#include"ObjectBase.h"

#include"ObjectServer.h"
#include"PhysWorld.h"

#include"ModeGame.h"
#include"LightsOut.h"

AILookAround::AILookAround(AIComponent* owner)
	:AIState(owner)
	,_frameCnt(0)
{

}

AILookAround::~AILookAround() {

}

void AILookAround::OnEnter() {
	_frameCnt = 120;
}

void AILookAround::OnExist() {

}

bool AILookAround::Process() {
	_frameCnt--;

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
					_owner->AddPoint("BackPatrolGoal", _owner->GetOwner()->GetPos());
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