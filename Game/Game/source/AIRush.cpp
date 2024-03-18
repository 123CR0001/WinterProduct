#include"AIRush.h"
#include"AIComponent.h"
#include"ObjectServer.h"
#include"Player.h"
#include"ObjectBase.h"

AIRush::AIRush(AIComponent* owner)
	:AIState(owner){

}

AIRush::~AIRush(){}

void AIRush::OnEnter() {
	_rushPos = _owner->GetChaseObject()->GetPos();
}

void AIRush::OnExist() {}

bool AIRush::Process() {

	//移動座標の更新
	{
		auto object = _owner->GetChaseObject();
		if (_owner->IsFound(object)) { _rushPos = object->GetPos(); }
	}

	Vector3 toObject = _rushPos - _owner->GetOwner()->GetPos();

	//移動する座標に、モデルの向きを変更
	_owner->GetOwner()->SetEulerAngle(Vector3(0.f, atan2f(toObject.x, toObject.z), 0.f));

	//移動
	_owner->GetOwner()->AddPos(toObject.Normalize() * 4.f);

	//突進したが、キャラがいない
	if (Vector3::LengthSquare(_rushPos, _owner->GetOwner()->GetPos()) < 10.f) {
		_owner->ChangeState("Trace");
	}

	return true;
}