#include"OrbitCamera.h"
#include"ObjectBase.h"
#include"appframe.h"
#include"ObjectServer.h"
#include"ModeGame.h"

OrbitCamera::OrbitCamera(ObjectBase* owner, int order)
	:CameraComponent(owner, order)
{
	_angle.y = 0.f;
	_angle.x = PI / 4.f;
	_dist = 400.f;
}

OrbitCamera::~OrbitCamera(){}

bool OrbitCamera::Process() {

	if (_angle.y >= 2.f * PI) { _angle.y = 0; }
	if (_angle.y < 0.f) { _angle.y = 2.f * PI; }

	auto key = _owner->GetObjectServer()->GetGame()->GetPad()->GetKeyButton();

	if (key & INPUT_DPAD_DOWN) { _dist += 1.f; }
	if (key & INPUT_DPAD_UP) { _dist -= 1.f; }
	if (_dist <= 0.f) { _dist = 0.f; }

	_angle.x = Clamp(0.1f,PI-0.1f, _angle.x);

	auto Rstick = _owner->GetObjectServer()->GetGame()->GetPad()->GetRightStick();
	if (Rstick.x > 30000) {
		_angle.y -= DegToRad(1);
	}
	if (Rstick.x < -30000) {
		_angle.y += DegToRad(1);
	}
	if (Rstick.y > 30000) {
		_angle.x -= DegToRad(1);
	}
	if (Rstick.y < -30000) {
		_angle.x += DegToRad(1);
	}

	Vector3D diff(
		_dist * sinf(_angle.x) * cosf(_angle.y-PI/2.f),
		_dist * cosf(_angle.x),
		_dist * sinf(_angle.y-PI/2.f) * sinf(_angle.x)
	);

	diff *= _targetDistMag;

	_pos = _owner->GetPos() + diff;

	//カメラにセット
	SetCameraPositionAndTarget_UpVecY(DxConverter::VecToDx(_pos), _owner->GetDxPos());

	return true;
}