
#include "AppFrame.h"
#include "ApplicationMain.h"
#include "ApplicationGlobal.h"
#include "ModeEffekseer.h"

bool ModeEffekseer::Initialize() {
	if (!base::Initialize()) { return false; }

	// エフェクトリソースを読み込む。
	// 読み込む時に大きさを指定する。ef_blood_02
	_effectHandle["Laser"] = LoadEffekseerEffect("res/effekseer/Laser01.efkefc", 10.0f);
	_effectHandle["Blood01"] = LoadEffekseerEffect("res/effekseer/ef_blood_01/ef_blood_01.efkefc", 50.f);
	_effectHandle["Blood02"] = LoadEffekseerEffect("res/effekseer/ef_blood_02/ef_blood_02.efkefc", 50.f);
	_effectHandle["Decoy"] = LoadEffekseerEffect("res/effekseer/ef_decoy_4/ef_decoy_4/ef_decoy.efkefc", 1.f);
	return true;
}

bool ModeEffekseer::Terminate() {
	base::Terminate();

	// エフェクトリソースを削除する。
	/*DeleteEffekseerEffect(_effectResourceHandle);*/

	for (auto&& handle : _effectHandle) {
		DeleteEffekseerEffect(handle.second);
	}

	return true;
}


bool ModeEffekseer::Process() {
	base::Process();

	// Effekseerにより再生中のエフェクトを更新する。
	UpdateEffekseer3D();

	return true;
}

bool ModeEffekseer::Render() {
	base::Render();

	// DXライブラリのカメラとEffekseerのカメラを同期する。
	Effekseer_Sync3DSetting();

	// Effekseerにより再生中のエフェクトを描画する。
	DrawEffekseer3D();


	return true;
}

int ModeEffekseer::Play(std::string name, const Vector3D& pos, const Vector3D& angle) {
	if (_effectHandle.find(name) != _effectHandle.end()) {
		int play = PlayEffekseer3DEffect(_effectHandle[name]);

		SetPosPlayingEffekseer3DEffect(play, pos.x, pos.y, pos.z);

		SetRotationPlayingEffekseer3DEffect(play, angle.x, angle.y, angle.z);

		return play;
	}
	//失敗
	return -1;
}

