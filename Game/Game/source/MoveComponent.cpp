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
	_oldEulerAngle = _owner->GetEulerAngle();

	//��]
	if (fabsf(_rotateSpeed) > 0.01f) {
		Vector3 angle = _owner->GetEulerAngle();
		angle.y += _rotateSpeed;
		_owner->SetEulerAngle(angle);
	}

	//�ړ�
	if (_moveSpeed > 0.01f) {
		float forward = _owner->GetEulerAngle().y;
		Vector3 vector(sinf(forward), 0.f, cosf(forward));
		Vector3 pos = _owner->GetPos();
		pos += vector * _moveSpeed;
		_owner->SetPos(pos);
	}

	//������
	_rotateSpeed = 0.f;
	_moveSpeed = 0.f;

	return true;
}

float MoveComponent::GetSpeed() const{ return Vector3::Length(_oldPos,_owner->GetPos()); }

Vector3 MoveComponent::GetRatateSpeed()const { 
	return _owner->GetEulerAngle() - _oldEulerAngle;
} 