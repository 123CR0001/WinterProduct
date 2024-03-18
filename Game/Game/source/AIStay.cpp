#include"AIStay.h"
#include"AIComponent.h"
#include"ObjectBase.h"
#include"ObjectServer.h"

#include"ModeGame.h"
#include"LightsOut.h"

AIStay::AIStay(AIComponent* owner)
	:AIState(owner)
	,_isSetForward(false)
{

}

AIStay::~AIStay(){}

void AIStay::OnEnter() {
	if (!_isSetForward) { _ownerForward = _owner->GetOwner()->GetForward(); _isSetForward = true; }
}

void AIStay::OnExist() {

}

bool AIStay::Process() {

	//音が聞こえたか？
	{
		Vector3 p;
		if(_owner->GetOwner()->GetObjectServer()->GetPhysWorld()->IsHear(_owner->GetOwner(), &p)) {
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

		for(int a = 0; a < objects.size(); a++) {

			for(int b = 0; b < names.size(); b++) {

				//登録した名前か
				if(objects[a]->GetName() != names[b]) { continue; }

				//視界に入っているか
				if(_owner->IsFound(objects[a])) {
					//追いかけるオブジェクトのアドレスを登録
					_owner->SetChaseObject(objects[a]);
					//AIStateを変更
					_owner->AddPoint("MoveTo", objects[a]->GetPos());
					_owner->ChangeState("Discovery");
					_owner->AddPoint("BackPatrolGoal", _owner->GetOwner()->GetPos());
					return true;

				}
			}
		}
	}

	//LightsOutになったら,AIBlindWalkに変更
	if(!_owner->GetOwner()->GetObjectServer()->GetGame()->GetLightsOut()->IsUse()) {
		_owner->ChangeState("BlindWalk");
	}

	float angle = Vector3::CrossAngleXZ(_owner->GetOwner()->GetForward(), _ownerForward);

	if(fabsf(angle) < DegToRad(3)) { return true; }

	if(angle < 0.f) {
		_owner->GetOwner()->AddEulerAngle(Vector3(0.f, DegToRad(1), 0.f));
	}
	else {
		_owner->GetOwner()->AddEulerAngle(Vector3(0.f, -DegToRad(1), 0.f));
	}

	return true;
}