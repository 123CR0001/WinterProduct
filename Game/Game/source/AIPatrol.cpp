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
	,_pointsNum(1)
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
		//目標座標に変化がないか
		int oldNum = _pointsNum;
		_owner->MoveTo(_owner->GetPoints(GetName()), _pointsNum);

		//ポイントに到着
		if (oldNum != _pointsNum) {
			_owner->ChangeState("LookAround");
		}
	}

	//AIStateを変更したか
	bool isChangeState = false;

	//音が聞こえたか？
	{
		Vector3 p;
		if (_owner->GetOwner()->GetObjectServer()->GetPhysWorld()->IsHear(_owner->GetOwner(), &p)) {
			isChangeState = true;

			//
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
			_owner->SetChaseObject(object);
			_owner->AddPoint("MoveTo", object->GetPos());
			_owner->ChangeState("Discovery");
			_owner->AddPoint("BackPatrolGoal", _owner->GetOwner()->GetPos());
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