
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
	_effectHandle["Blood02"] = LoadEffekseerEffect("res/effekseer/ef_blood_pool/ef_blood_pool.efkefc", 150.f);
	_effectHandle["Decoy"] = LoadEffekseerEffect("res/effekseer/ef_decoy_4/ef_decoy_4/ef_decoy.efkefc", 1.f);
	_effectHandle["GetEnergy"] = LoadEffekseerEffect("res/effekseer/ef_item_01/ef_item_01.efkefc", 30.f);
	_effectHandle["bikkuri"] = LoadEffekseerEffect("res/effekseer/bikkuri/bikkuri.efkefc", 100.f);
	_effectHandle["question_mark"] = LoadEffekseerEffect("res/effekseer/question_mark/question mark.efkefc", 100.f);
	_effectHandle["MuzzleFlash"] = LoadEffekseerEffect("res/effekseer/ef_muzzleflash_01/ef_muzzleflash.efkefc", 10.f);
	return true;
}

bool ModeEffekseer::Terminate() {
	base::Terminate();

	// �G�t�F�N�g���\�[�X���폜����B
	/*DeleteEffekseerEffect(_effectResourceHandle);*/

	for (auto&& handle : _effectHandle) {
		DeleteEffekseerEffect(handle.second);
	}

	for(auto&& handle : _playingEffectHandles) {
		StopEffekseer3DEffect(handle);
	}

	return true;
}


bool ModeEffekseer::Process() {
	base::Process();

	// Effekseer�ɂ��Đ����̃G�t�F�N�g���X�V����B
	UpdateEffekseer3D();

	for(auto iter = _playingEffectHandles.begin(); iter != _playingEffectHandles.end();) {
		if(IsEffekseer3DEffectPlaying((*iter)) == 0) {
			++iter;
		}
		else {
			iter = _playingEffectHandles.erase(iter);
		}
	}
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

int ModeEffekseer::Play(std::string name, const Vector3& pos, const Vector3& angle) {
	if (_effectHandle.find(name) != _effectHandle.end()) {
		int play = PlayEffekseer3DEffect(_effectHandle[name]);

		SetPosPlayingEffekseer3DEffect(play, pos.x, pos.y, pos.z);

		SetRotationPlayingEffekseer3DEffect(play, angle.x, angle.y + PI , angle.z);

		_playingEffectHandles.emplace_back(play);

		return play;
	}
	//���s
	return -1;
}

