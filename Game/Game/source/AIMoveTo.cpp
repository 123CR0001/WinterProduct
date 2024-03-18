#include"AIMoveTo.h"
#include"AIComponent.h"
#include"ObjectServer.h"
#include"appframe.h"
#include"CommonSoldier.h"
#include"Player.h"
#include"ModeGame.h"
#include"LightsOut.h"
AIMoveTo::AIMoveTo(AIComponent* owner)
	:AIState(owner)
	, _pointsNum(0)
	, _frameCnt(0)
{
}
AIMoveTo::~AIMoveTo() {}
void AIMoveTo::OnEnter() {
	if(_owner->GetPoints(GetName()).empty()) { _owner->ChangeState("LookAround"); return; }
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
		_owner->ChangeState("LookAround");
		return;
	}
	_oldCheckPoint = checkPoint;
	_frameCnt = 0;
}
void AIMoveTo::OnExist() {
}
bool AIMoveTo::Process() {
	//何か見かけたか
	bool isFound = false;
	if(_owner->GetPoints(GetName()).size() == 0) {
		_owner->ChangeState("LookAround");
	}
	//移動　最後の座標まで到達したら、巡回経路に戻る
	if(!_owner->GetPoints(GetName()).back().Equal(_owner->GetOwner()->GetPos(), 20.f)) {
		//移動
		_owner->MoveTo(_owner->GetPoints(GetName()), _pointsNum);
	}
	else {
		if(!isFound) {
			_owner->ChangeState("LookAround");
		}
	}
	//登録している名前と同じ名前を持つオブジェクトを視界に入れたか
	{
		auto objects = _owner->GetOwner()->GetObjectServer()->GetObjects();
		std::vector<std::string>names;
		names.emplace_back("player");
		names.emplace_back("Decoy");
		for(int a = 0; a < objects.size(); a++) {
			//視界に入っていない
			if(!_owner->IsFound(objects[a])) { continue; }
			for(int b = 0; b < names.size(); b++) {
				if(objects[a]->GetName() == names[b]) {
					//目的地を更新
					if(_oldCheckPoint != objects[a]->GetPos()) {
						_oldCheckPoint = _owner->GetPoints(GetName()).back();
						_owner->AddPoint(GetName(), objects[a]->GetPos());
						_owner->ChangeState(GetName());
					}
					isFound = true;
					break;
				}
			}
		}
	}
	//音が聞こえたか？
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