#include"AIPatrol.h"
#include"AIComponent.h"
#include"ObjectBase.h"
#include"CommonSoldier.h"
#include"ObjectServer.h"
#include<algorithm>
#include"AIBackPatrol.h"
#include"PhysWorld.h"
#include"Player.h"
#include"AIChase.h"

AIPatrol::AIPatrol(AIComponent* owner)
	:AIState(owner)
	,_patrolPointsNum(1)
{}

AIPatrol::~AIPatrol(){}


void AIPatrol::OnEnter() {
}

void AIPatrol::OnExist() {

}

bool AIPatrol::Process() {

	auto owner = _owner->GetOwner();
	auto objectServer = _owner->GetOwner()->GetObjectServer();

	//巡回する経路がないので、AIStayに切り替える
	if(_owner->GetPoints(GetName()).size() == 1) { 	_owner->ChangeState("Stay");}

	//経路を巡回
	{
		int oldNum = _patrolPointsNum;
		_owner->MoveTo(_owner->GetPoints(GetName()), _patrolPointsNum);

		//ポイントに到着
		if (oldNum != _patrolPointsNum) {
			_owner->ChangeState("LookAround");
		}
	}


	//音が聞こえたか？
	{
		Vector3D p;
		if (_owner->GetOwner()->GetObjectServer()->GetPhysWorld()->IsHear(_owner->GetOwner(), &p)) {
			_owner->DeletePoint("CheckPoint");
			_owner->AddPoint("CheckPoint", p);
			_owner->ChangeState("CheckPoint");
		}
	}

	//登録している名前と同じ名前を持つオブジェクトを視界に入れたか
	{
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
					//追いかけるオブジェクトのアドレスを登録
					_owner->SetChaseObject(objects[a]);
					//AIStateを変更
					_owner->ChangeState("Chase");
					return true;
				
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