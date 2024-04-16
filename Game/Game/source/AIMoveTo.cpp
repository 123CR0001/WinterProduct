#include"AIMoveTo.h"
#include"AIComponent.h"
#include"ObjectServer.h"
#include"appframe.h"
#include"CommonSoldier.h"
#include"Player.h"
#include"ModeGame.h"
#include"LightsOut.h"

constexpr int INTERVAL = 300;

AIMoveTo::AIMoveTo(AIComponent* owner)
	:AIState(owner)
	, _pointsNum(0)
	, _interval(0)
{
}
AIMoveTo::~AIMoveTo() {}
void AIMoveTo::OnEnter() {
	if(_owner->GetPoints(GetName()).empty()) { _owner->ChangeState("LoseSight"); return; }
	//一番最後に追加された座標へ向かう
	Vector3 checkPoint = _owner->GetPoints(GetName()).back();
	//昔のルートは捨てる
	_pointsNum = 0;
	_owner->GetPoints(GetName()).clear();
	//このAIStateを所持するAIComponentを所持するObjectBaseが所属するServer
	auto server = _owner->GetOwner()->GetObjectServer();
	auto owner = _owner->GetOwner();
	auto navi = _owner->GetOwner()->GetObjectServer()->GetNavi();
	auto ownerPos = owner->GetPos();
	//最短経路
	navi->BFS(ownerPos, checkPoint, _owner->GetPoints(GetName()));
	if(_owner->GetPoints(GetName()).empty()) {
		_owner->ChangeState("LoseSight");
		return;
	}
	_oldCheckPoint = checkPoint;
	_interval = INTERVAL;
}
void AIMoveTo::OnExist() {
}
bool AIMoveTo::Process() {
	//何か見かけたか
	bool isFound = false;
	if(_owner->GetPoints(GetName()).size() == 0) {
		_owner->ChangeState("LoseSight");
	}
	//移動　最後の座標まで到達したら、巡回経路に戻る
	if(!Vector3::Equal(_owner->GetPoints(GetName()).back(),_owner->GetOwner()->GetPos(), 20.f)) {
		//移動
		_owner->MoveTo(_owner->GetPoints(GetName()), _pointsNum);
	}
	else {
		if(!isFound) {
			_owner->ChangeState("LoseSight");
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

	//一定時間経過したら、音が聞こえたかを確認
	if(_interval == 0) {
		Vector3 p;
		//音が聞こえたか？
		if(_owner->GetOwner()->GetObjectServer()->GetPhysWorld()->IsHear(_owner->GetOwner(), &p)) {
			//音の発生源の座標を登録
			_owner->AddPoint("MoveTo", p);

			//音が聞こえたら、Discoveryに変更
			_owner->ChangeState("Discovery");

			//今いる座標を戻る座標として登録
			_owner->AddPoint("BackPatrolGoal", _owner->GetOwner()->GetPos());
		}
	}
	//LightsOutになったら,AIBlindWalkに変更
	if(!_owner->GetOwner()->GetObjectServer()->GetGame()->GetLightsOut()->IsUse()) {
		_owner->ChangeState("BlindWalk");
	}

	//インターバルの更新
	if(_interval > 0) {
		_interval--;
	}
	return true;
}