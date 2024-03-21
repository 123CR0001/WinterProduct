#include "SoundItem.h"
#include "SoundServer.h"

SoundItemBase::SoundItemBase(std::string filename, int flg) {
	_filename = filename;
	_flg = flg;
	_snd = -1;
	_volume = 160;
	_pan = 0;
	_frequency = 0;		// ファイルによって標準が違うので
	_sndServer = NULL;
}
SoundItemBase::~SoundItemBase() {
	Unload();
}

void	SoundItemBase::Unload() {
	if (_snd != -1) {
		DeleteSoundMem(_snd);
		_snd = -1;
	}
}

bool	SoundItemBase::IsLoad() {
	// 非同期ロードが終わっているか？
	if (_snd != -1 && CheckHandleASyncLoad(_snd) == FALSE) {
		return true;
	}
	return false;
}

bool	SoundItemBase::IsPlay() {
	if (_snd != -1 && CheckSoundMem(_snd) == 1) {
		return true;
	}
	return false;
}

void	SoundItemBase::Stop() {
	if (_snd != -1) {
		StopSoundMem(_snd);
	}
}

int		SoundItemBase::GetVolume() { return _volume; }
void	SoundItemBase::SetVolume(int volume) {
	_volume = volume;
	if (_snd != -1) {
		ChangeVolumeSoundMem(_volume, _snd);
	}
}
int		SoundItemBase::GetPan() { return _pan; }
void	SoundItemBase::SetPan(int pan) {
	_pan = pan;
	if (_snd != -1) {
		ChangePanSoundMem(_pan, _snd);
	}
}

int		SoundItemBase::GetFrequency() { return _frequency; }
void	SoundItemBase::SetFrequency(int frequency) {
	_frequency = frequency;
	if (_snd != -1 && _frequency != 0) {
		SetFrequencySoundMem(_frequency, _snd);
	}
}
void	SoundItemBase::ResetFrequency() {
	_frequency = 0;
	if (_snd != -1) {
		ResetFrequencySoundMem(_snd);
		_frequency = GetFrequencySoundMem(_snd);
	}
}

void	SoundItemBase::PlayMem(int flg) {
	// 再生前に音量等を設定
	SetVolume(_volume);
	SetPan(_pan);
	SetFrequency(_frequency);
	PlaySoundMem(_snd, flg, TRUE);
}

int		SoundItemBase::LoadMem(std::string filename) {
	if (_flg & FLG_3D) {
		SetCreate3DSoundFlag(TRUE);
	}
	int snd = LoadSoundMem(filename.c_str());
	SetCreate3DSoundFlag(FALSE);
	return snd;
}




void	SoundItemBGM::Play() {
	if (_sndServer) {
		_sndServer->StopType(TYPE::BGM);
	}
	StreamLoad();	// ストリーム再生のものをロードする
	if (IsPlay() == false) {	// BGMは再生されていない場合のみ
		PlayMem(DX_PLAYTYPE_LOOP);
	}
}

void	SoundItemOneShot::Update() {
	int snd = _snd;
	base::Update();
	if (snd != -1 && _snd == -1) {
		// 再生されていたがUnload()されている場合、自分を削除
		_sndServer->Del(this);
	}
}
