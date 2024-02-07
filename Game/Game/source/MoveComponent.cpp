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

	//回転
	if (fabsf(_rotateSpeed) > 0.01f) {
		Vector3D angle = _owner->GetEulerAngle();
		angle.y += _rotateSpeed;
		//if (angle.y > 2.f * PI) { angle.y -= 2 * PI; }
		//if (angle.y < 0.f) { angle.y *= -1; }
		_owner->SetEulerAngle(angle);
	}

	//移動
	if (_moveSpeed > 0.01f) {
		float forward = _owner->GetEulerAngle().y;
		Vector3D vector(sinf(forward), 0.f, cosf(forward));
		Vector3D pos = _owner->GetPos();
		pos += vector * _moveSpeed;
		_owner->SetPos(pos);
	}

	//初期化
	_rotateSpeed = 0.f;
	_moveSpeed = 0.f;

	return true;
}

float MoveComponent::GetSpeed() { return Vector3D(_owner->GetPos() - _oldPos).Length(); }