#include"OrbitCamera.h"
#include"ObjectBase.h"
#include"appframe.h"
#include"ObjectServer.h"
#include"ModeGame.h"

OrbitCamera::OrbitCamera(ObjectBase* owner, int order)
	:CameraComponent(owner, order)
{
	_yaw = 0.f;
	_pitch = PI / 2.f;
	_dist = 400.f;
}

OrbitCamera::~OrbitCamera(){}

bool OrbitCamera::Process() {

	if (_yaw >= 2.f * PI) { _yaw = 0; }
	if (_yaw < 0.f) { _yaw = 2.f * PI; }

	auto key = _owner->GetObjectServer()->GetGame()->GetPad()->GetKeyButton();

	if (key & INPUT_DPAD_DOWN) { _dist += 1.f; }
	if (key & INPUT_DPAD_UP) { _dist -= 1.f; }
	if (_dist <= 0.f) { _dist = 0.f; }

	_pitch = Clamp(0.1f,PI-0.1f, _pitch);

	auto Rstick = _owner->GetObjectServer()->GetGame()->GetPad()->GetRightStick();
	if (Rstick.x > 30000) {
		_yaw -= DegToRad(1);
	}
	if (Rstick.x < -30000) {
		_yaw += DegToRad(1);
	}
	if (Rstick.y > 30000) {
		_pitch -= DegToRad(1);
	}
	if (Rstick.y < -30000) {
		_pitch += DegToRad(1);
	}

	Vector3D pos(
		_dist * sinf(_pitch) * cosf(_yaw),
		_dist * cosf(_pitch),
		_dist * sinf(_pitch) * sinf(_yaw)
	);

	_pos = _owner->GetPos() + pos;

	//カメラにセット
	SetCameraPositionAndTarget_UpVecY(DxConverter::VecToDx(_pos), _owner->GetDxPos());

	return true;
}