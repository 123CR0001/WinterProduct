#include"AIPatrol.h"
#include"AIComponent.h"
#include"ObjectBase.h"
#include"CommonSoldier.h"
#include"ObjectServer.h"
#include<algorithm>
#include"AIBackPatrol.h"
#include"PhysWorld.h"
#include"Player.h"

#include"ModeGame.h"
#include"LightsOut.h"

AIPatrol::AIPatrol(AIComponent* owner)
	:AIState(owner)
	,_patrolPointsNum(1)
{}

AIPatrol::~AIPatrol(){}


void AIPatrol::OnEnter() {
	//巡回する経路がないので、AIStayに切り替える
	if(_owner->GetPoints(GetName()).size() == 1) { _owner->ChangeState("Stay"); }
}

void AIPatrol::OnExist() {

}

bool AIPatrol::Process() {

	auto owner = _owner->GetOwner();
	auto objectServer = _owner->GetOwner()->GetObjectServer();

	//経路を巡回
	{
		int oldNum = _patrolPointsNum;
		_owner->MoveTo(_owner->GetPoints(GetName()), _patrolPointsNum);

		//ポイントに到着
		if (oldNum != _patrolPointsNum) {
			_owner->ChangeState("LookAround");
		}
	}

	//AIStateを変更したか
	bool isChangeState = false;

	//音が聞こえたか？
	{
		Vector3D p;
		if (_owner->GetOwner()->GetObjectServer()->GetPhysWorld()->IsHear(_owner->GetOwner(), &p)) {
			isChangeState = true;
			_owner->DeletePoint("CheckPoint");
			_owner->AddPoint("CheckPoint", p);
			_owner->ChangeState("Discovery");
			_owner->AddPoint("BackPatrolGoal", _owner->GetOwner()->GetPos());
		}
	}

	//登録している名前と同じ名前を持つオブジェクトを視界に入れたか
	
		auto objects = _owner->GetOwner()->GetObjectServer()->GetObjects();
		std::vector<std::string>names;

		names.emplace_back("player");
		names.emplace_back("Decoy");

		for (int a = 0; a < objects.size(); a++) {

			for (int b = 0; b < names.size(); b++) {

				//登録した名前か
				if (objects[a]->GetName() != names[b]) { continue; }

				//視界に入っているか
				if (_owner->IsFound(objects[a])) { 

					isChangeState = true;
					//追いかけるオブジェクトのアドレスを登録
					_owner->SetChaseObject(objects[a]);
					//AIStateを変更
					_owner->AddPoint("CheckPoint", objects[a]->GetPos());
					_owner->ChangeState("Discovery");
					_owner->AddPoint("BackPatrolGoal", _owner->GetOwner()->GetPos());
					return true;
				
				}
			}
		}
	

	//LightsOutになったら,AIBlindWalkに変更
	if(!_owner->GetOwner()->GetObjectServer()->GetGame()->GetLightsOut()->IsUse()) {
		_owner->ChangeState("BlindWalk");
	}


	//AIStateが変更されていたら、目的地の座標をAIBackPatrolの座標コンテナに登録
	//この登録した座標を基に、元の位置に戻ってくる

	
	return true;
}