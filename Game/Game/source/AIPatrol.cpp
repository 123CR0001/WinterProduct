#include"AIPatrol.h"
#include"AIComponent.h"
#include"ObjectBase.h"
#include"CommonSoldier.h"
#include"ObjectServer.h"
#include<algorithm>
#include"AIBackPatrol.h"
#include"PhysWorld.h"

AIPatrol::AIPatrol(AIComponent* owner, AIBackPatrol* AI)
	:AIState(owner)
	,_AIBackPatrol(AI)
	,_patrolPointsNum(0)
{}

AIPatrol::~AIPatrol(){}


void AIPatrol::OnEnter() {
	_AIBackPatrol->ClearBackPatrol();
}

void AIPatrol::OnExist() {

}

bool AIPatrol::Process() {

	if (_patrolPoints.size() == 0) {
		//ó‘Ô‚ð‘JˆÚ‚·‚é
		//owner->ChangeState("GateKeeper");
	}

	//Server‚Ì_enemys‚É“o˜^‚³‚ê‚Ä‚¢‚é‚©
	auto iter = std::find(_owner->GetOwner()->GetObjectServer()->GetCommonSoldiers().begin(),
		_owner->GetOwner()->GetObjectServer()->GetCommonSoldiers().end(),
		_owner->GetOwner()
	);

	//‚³‚ê‚Ä‚¢‚½‚çAˆ—‚ð‚·‚é
	if (iter != _owner->GetOwner()->GetObjectServer()->GetCommonSoldiers().end()) {
		(*iter)->MoveRoute(_patrolPoints, _patrolPointsNum);
		if ((*iter)->IsPlayerFound()) {
			_owner->ChangeState("ChasePlayer");
		}
	}

	//‰¹‚ª•·‚±‚¦‚½‚©H
	Vector3D p;
	if (_owner->GetOwner()->GetObjectServer()->GetPhysWorld()->IsHear(_owner->GetOwner(), p)) {
		_owner->AddPoint("CheckPoint", p);
		_owner->ChangeState("CheckPoint");
	}
	
	return true;
}

void AIPatrol::SetData(void* data) {

	//ˆê•àŠÔˆá‚Á‚½‚çƒGƒ‰[‚ªo‚é
	//³’¼•|‚¢

	nlohmann::json* castData = (nlohmann::json*)data;

	char num[10];
	int count = 1;

	for (auto& marker : *castData) {

		std::string name = "marker";

		snprintf(num, 8, "%d", count);
		name += num;
		count++;

		if (marker.at("objectName") == name) {
			Vector3D pos(marker.at("translate").at("x"), marker.at("translate").at("z"), -1 * marker.at("translate").at("y"));
			_patrolPoints.emplace_back(pos);

			/*_scale = Vector3D(marker.at("scale").at("x"), marker.at("scale").at("z"), marker.at("scale").at("y"));*/
		}

	}
	if (_patrolPoints.size() > 0) {
		_owner->GetOwner()->SetPos(_patrolPoints.front());
		_patrolPointsNum = 1;
	}
}