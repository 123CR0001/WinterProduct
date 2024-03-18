#include"FollowCamera.h"
#include"ObjectBase.h"

FollowCamera::FollowCamera(ObjectBase* owner, int order)
	:CameraComponent(owner, order)
	,_springConst(100.f)
	,_widthDist(250.f)
	,_heightDist(800.f)
{
	_angle = Vector3(0.f, DegToRad(-90.f), 0.f);
}

FollowCamera::~FollowCamera(){}

bool FollowCamera::Initialize() {
	//最初に理想とする位置に移動
	float backwardAngle = DegToRad(180) - _owner->GetEulerAngle().y;
	Vector3 ideal = Vector3(sin(backwardAngle), 0.f, cos(backwardAngle)) * _widthDist;
	ideal.y = _heightDist;
	ideal += _owner->GetPos();
	_pos = ideal;

	return true;
}

bool FollowCamera::Process() {

	CameraComponent::Process();

	float dampening = 2.0f * sqrt(_springConst);

	Vector3 idealPos = Vector3(sinf(_angle.y), 0.f, cosf(_angle.y)) * (_widthDist * _targetDistMag);

	idealPos.y = _heightDist * _targetDistMag;
	idealPos += _owner->GetPos();

	Vector3 diff = _pos - idealPos;

	//ma = -Kx	ばね定数
	Vector3 accele = Vector3(diff * -_springConst) - Vector3(_velocity * dampening);

	//m = m / (s*s) * (s*s)
	_velocity += accele * (1.f / 60.f);

	//移動
	_pos += _velocity * (1.f / 60.f);

	Vector3 target = _owner->GetPos() - Vector3(idealPos - _pos);

	//カメラにセット
	SetCameraPositionAndTarget_UpVecY(DxConverter::VecToDx(_pos), DxConverter::VecToDx(target));

	return true;
}