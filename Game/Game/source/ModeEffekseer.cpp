
#include "AppFrame.h"
#include "ApplicationMain.h"
#include "ApplicationGlobal.h"
#include "ModeEffekseer.h"

bool ModeEffekseer::Initialize() {
	if (!base::Initialize()) { return false; }

	// �G�t�F�N�g���\�[�X��ǂݍ��ށB
	// �ǂݍ��ގ��ɑ傫�����w�肷��Bef_blood_02
	_effectHandle["Laser"] = LoadEffekseerEffect("res/effekseer/Laser01.efkefc", 10.0f);
	_effectHandle["Blood01"] = LoadEffekseerEffect("res/effekseer/ef_blood_01/ef_blood_01.efkefc", 50.f);
	_effectHandle["Blood02"] = LoadEffekseerEffect("res/effekseer/ef_blood_02/ef_blood_02.efkefc", 50.f);
	_effectHandle["Decoy"] = LoadEffekseerEffect("res/effekseer/ef_decoy_4/ef_decoy_4/ef_decoy.efkefc", 1.f);
	return true;
}

bool ModeEffekseer::Terminate() {
	base::Terminate();

	// �G�t�F�N�g���\�[�X���폜����B
	/*DeleteEffekseerEffect(_effectResourceHandle);*/

	for (auto&& handle : _effectHandle) {
		DeleteEffekseerEffect(handle.second);
	}

	return true;
}


bool ModeEffekseer::Process() {
	base::Process();

	// Effekseer�ɂ��Đ����̃G�t�F�N�g���X�V����B
	UpdateEffekseer3D();

	return true;
}

bool ModeEffekseer::Render() {
	base::Render();

	// DX���C�u�����̃J������Effekseer�̃J�����𓯊�����B
	Effekseer_Sync3DSetting();

	// Effekseer�ɂ��Đ����̃G�t�F�N�g��`�悷��B
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
	//���s
	return -1;
}

