#include"Gun.h"
#include"CharaBase.h"
#include"ObjectServer.h"
#include"ModeGame.h"
#include"ModeEffekseer.h"

Gun::Gun(CharaBase* chara)
	:WeaponBase(chara)
{


	_scale = Vector3(3.8f, 3.8f, 3.8f);
	_eulerAngle = Vector3(4.62f,0.18f,5.47f);
	_pos = Vector3(0.f,-8.62f,-1.72);
}

Gun::~Gun(){}

bool Gun::Initialize() {
	WeaponBase::Initialize();
	LoadModel("res/Object/Gun/Gun.mv1", "muzzle");
	return true;
}

bool Gun::Process() {
	WeaponBase::Process();
	{
		int FrameIndex;
		_matrix = MGetIdent();
		// フレーム名からフレーム番号を取得する
		FrameIndex = MV1SearchFrame(_equippedChara->GetHandle(), "Soldier1_RightHand");
		_matrix = MGetScale(DxConverter::VecToDx(_scale));
		_matrix = MMult(_matrix, MGetRotX(_eulerAngle.x));
		_matrix = MMult(_matrix, MGetRotX(_eulerAngle.y));
		_matrix = MMult(_matrix, MGetRotZ(_eulerAngle.z));
		_matrix = MMult(_matrix, MGetTranslate(DxConverter::VecToDx(_pos)));
		_matrix = MMult(_matrix, MV1GetFrameLocalWorldMatrix(_equippedChara->GetHandle(),FrameIndex));
		MV1SetMatrix(_handle, _matrix);
	}
	if (_isAttack) {
		int frameIndex = MV1SearchFrame(_handle, "muzzle");

		//エフェクト再生
		GetObjectServer()->GetGame()->GetModeEffekseer()->Play(
			"MuzzleFlash",
			DxConverter::DxToVec(MV1GetFramePosition(_handle, frameIndex)),
			_equippedChara->GetEulerAngle() + Vector3(0.f, -PI / 2.f , 0.f)
		);


		_isAttack = false;
	}


	return true;
}
