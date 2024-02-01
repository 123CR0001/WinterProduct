#include"FollowCamera.h"
#include"ObjectBase.h"

FollowCamera::FollowCamera(ObjectBase* owner, int order)
	:CameraComponent(owner, order)
	,_springConst(0.7f)
	,_vDist(250.f)
	,_heightDist(800.f)
	,_forwardAngle(DegToRad(270.f))
{

}

FollowCamera::~FollowCamera(){}

bool FollowCamera::Initialize() {
	//最初に理想とする位置に移動
	float backwardAngle = DegToRad(180) - _owner->GetEulerAngle().y;
	Vector3D ideal = Vector3D(sin(backwardAngle), 0.f, cos(backwardAngle)) * _vDist;
	ideal.y = _heightDist;
	ideal += _owner->GetPos();
	_pos = ideal;

	return true;
}

bool FollowCamera::Process() {

	CameraComponent::Process();

	Vector3D idealPos = Vector3D(sin(_forwardAngle), 0.f, cos(_forwardAngle)) * _vDist;
	idealPos.y = _heightDist;
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