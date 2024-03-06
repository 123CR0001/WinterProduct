#include"AIStay.h"
#include"AIComponent.h"
#include"ObjectBase.h"
#include"ObjectServer.h"

AIStay::AIStay(AIComponent* owner)
	:AIState(owner)
	,_ownerEulerAngle(nullptr)
{

}

AIStay::~AIStay(){}

void AIStay::OnEnter() {
	if(!_ownerEulerAngle)_ownerEulerAngle = std::make_unique<Vector3D>(_owner->GetOwner()->GetEulerAngle());
}

void AIStay::OnExist() {

}

bool AIStay::Process() {

	//音が聞こえたか？
	{
		Vector3D p;
		if(_owner->GetOwner()->GetObjectServer()->GetPhysWorld()->IsHear(_owner->GetOwner(), &p)) {
			_owner->AddPoint("CheckPoint", p);
			_owner->ChangeState("CheckPoint");
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
					_owner->ChangeState("Chase");
					_owner->AddPoint("BackPatrolGoal", _owner->GetOwner()->GetPos());
					return true;

				}
			}
		}
	}

	//LightsOutになったら,AIBlindWalkに変更
	if(ModeServer::GetInstance()->IsAdd("LightsOut")) {
		_owner->ChangeState("BlindWalk");
	}

	_owner->GetOwner()->SetEulerAngle(*_ownerEulerAngle);

	//const Vector3D vCross = Vector3D::Cross(_owner->GetOwner()->GetForward(), *_ownerForward);

	//if(fabsf(sinf(vCross.z)) < DegToRad(3)) { return true; }

	//if(vCross.z < 0.f) {
	//	_owner->GetOwner()->AddEulerAngle(Vector3D(0.f, -DegToRad(1), 0.f));
	//}
	//else {
	//	_owner->GetOwner()->AddEulerAngle(Vector3D(0.f, DegToRad(1), 0.f));
	//}

	return true;
}