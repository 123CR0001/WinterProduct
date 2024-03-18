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

	//�ړ����W�̍X�V
	{
		auto object = _owner->GetChaseObject();
		if (_owner->IsFound(object)) { _rushPos = object->GetPos(); }
	}

	Vector3 toObject = _rushPos - _owner->GetOwner()->GetPos();

	//�ړ�������W�ɁA���f���̌�����ύX
	_owner->GetOwner()->SetEulerAngle(Vector3(0.f, atan2f(toObject.x, toObject.z), 0.f));

	//�ړ�
	_owner->GetOwner()->AddPos(toObject.Normalize() * 4.f);

	//�ːi�������A�L���������Ȃ�
	if (Vector3::LengthSquare(_rushPos, _owner->GetOwner()->GetPos()) < 10.f) {
		_owner->ChangeState("Trace");
	}

	return true;
}