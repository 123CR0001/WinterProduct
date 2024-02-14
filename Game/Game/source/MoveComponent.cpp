#include"MoveComponent.h"
#include"ObjectBase.h"

MoveComponent::MoveComponent(ObjectBase* owner,int order)
:Component(owner,order)
,_rotateSpeed(0.f)
,_moveSpeed(0.f)
{
}

MoveComponent::~MoveComponent(){}

bool MoveComponent::Process() {

	//�ړ��O�̈ʒu���W��ێ�
	_oldPos = _owner->GetPos();

	//��]
	if (fabsf(_rotateSpeed) > 0.01f) {
		Vector3D angle = _owner->GetEulerAngle();
		angle.y += _rotateSpeed;
		_owner->SetEulerAngle(angle);
	}

	//�ړ�
	if (_moveSpeed > 0.01f) {
		float forward = _owner->GetEulerAngle().y;
		Vector3D vector(sinf(forward), 0.f, cosf(forward));
		Vector3D pos = _owner->GetPos();
		pos += vector * _moveSpeed;
		_owner->SetPos(pos);
	}

	//������
	_rotateSpeed = 0.f;
	_moveSpeed = 0.f;

	return true;
}

float MoveComponent::GetSpeed() { return Vector3D(_owner->GetPos() - _oldPos).Length(); }