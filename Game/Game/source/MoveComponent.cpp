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

	//移動前の位置座標を保持
	_oldPos = _owner->GetPos();
	_oldEulerAngle = _owner->GetEulerAngle();

	//回転
	if (fabsf(_rotateSpeed) > 0.01f) {
		Vector3 angle = _owner->GetEulerAngle();
		angle.y += _rotateSpeed;
		_owner->SetEulerAngle(angle);
	}

	//移動
	if (_moveSpeed > 0.01f) {
		float forward = _owner->GetEulerAngle().y;
		Vector3 vector(sinf(forward), 0.f, cosf(forward));
		Vector3 pos = _owner->GetPos();
		pos += vector * _moveSpeed;
		_owner->SetPos(pos);
	}

	//初期化
	_rotateSpeed = 0.f;
	_moveSpeed = 0.f;

	return true;
}

float MoveComponent::GetSpeed() const{ return Vector3::Length(_oldPos,_owner->GetPos()); }

Vector3 MoveComponent::GetRotateSpeed()const { return _owner->GetEulerAngle() - _oldEulerAngle; } 