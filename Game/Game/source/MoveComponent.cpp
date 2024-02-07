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

	if (fabsf(_rotateSpeed) > 0.01f) {
		Vector3D angle = _owner->GetEulerAngle();
		angle.y += _rotateSpeed;
		//if (angle.y > 2.f * PI) { angle.y -= 2 * PI; }
		//if (angle.y < 0.f) { angle.y *= -1; }
		_owner->SetEulerAngle(angle);
	}

	if (_moveSpeed > 0.01f) {
		float forward = _owner->GetEulerAngle().y;
		Vector3D vector(sinf(forward), 0.f, cosf(forward));
		Vector3D pos = _owner->GetPos();
		pos += vector * _moveSpeed;
		_owner->SetPos(pos);
	}

	_rotateSpeed = 0.f;
	_moveSpeed = 0.f;

	return true;
}