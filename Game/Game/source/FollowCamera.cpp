#include"FollowCamera.h"
#include"ObjectBase.h"

FollowCamera::FollowCamera(ObjectBase* owner, int order)
	:CameraComponent(owner, order)
	,_springConst(0.7f)
	,_widthDist(250.f)
	,_heightDist(800.f)
{
	_angle = Vector3D(0.f, DegToRad(-90.f), 0.f);
}

FollowCamera::~FollowCamera(){}

bool FollowCamera::Initialize() {
	//最初に理想とする位置に移動
	float backwardAngle = DegToRad(180) - _owner->GetEulerAngle().y;
	Vector3D ideal = Vector3D(sin(backwardAngle), 0.f, cos(backwardAngle)) * _widthDist;
	ideal.y = _heightDist;
	ideal += _owner->GetPos();
	_pos = ideal;

	return true;
}

bool FollowCamera::Process() {

	CameraComponent::Process();

	Vector3D idealPos = Vector3D(sinf(_angle.y), 0.f, cosf(_angle.y)) * (_widthDist * _targetDistMag);
	idealPos.y = _heightDist * _targetDistMag;
	idealPos += _owner->GetPos();

	float diff = Vector3D::Length(idealPos, _pos);

	//ma = -Kx	ばね定数
	float accele = _springConst * diff;

	//m = m / (s*s) * (s*s)
	float speed = (float)(accele * (1.f / 60.f));
	float scale = (float)(speed * (1.f / 60.f));

	//移動
	_pos = (_pos + (Vector3D(idealPos - _pos) * scale));

	Vector3D target = _owner->GetPos() - Vector3D(idealPos - _pos);

	//カメラにセット
	SetCameraPositionAndTarget_UpVecY(DxConverter::VecToDx(_pos), DxConverter::VecToDx(target));

	return true;
}