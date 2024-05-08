#include"AIStay.h"
#include"AIComponent.h"
#include"ObjectBase.h"
#include"ObjectServer.h"

#include"ModeGame.h"

AIStay::AIStay(AIComponent* owner)
	:AIState(owner)
	,_isSetForward(false)
{

}

AIStay::~AIStay(){}

void AIStay::OnEnter() {
	//一回でも角度を設定したら、その角度がこのステートになる限り、その角度に変化する
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
		auto object = _owner->IsFound();

		if (object) {
			//AIStateを変更
			_owner->AddPoint("MoveTo", object->GetPos());
			_owner->ChangeState("Discovery");
			_owner->AddPoint("BackPatrolGoal", _owner->GetOwner()->GetPos());
		}
	}

	//LightsOutになったら、AIBlindWalkに変更
	if(_owner->GetOwner()->GetObjectServer()->GetGame()->IsUsingLightsOut()) {
		_owner->ChangeState("BlindWalk");
	}

	//設定されている(_ownerForward)の角度へ少しずつ向く
	float angle = Vector3::CrossAngleXZ(_owner->GetOwner()->GetForward(), _ownerForward);

	//角度が一定以下になったら、処理を終了
	if(fabsf(angle) < DegToRad(3)) { return true; }

	//角度がプラスかマイナスかで回転方向を変える
	if(angle < 0.f) {
		_owner->GetOwner()->AddEulerAngle(Vector3(0.f, DegToRad(1), 0.f));
	}
	else {
		_owner->GetOwner()->AddEulerAngle(Vector3(0.f, -DegToRad(1), 0.f));
	}

	return true;
}