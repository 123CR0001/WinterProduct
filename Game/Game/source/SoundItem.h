#pragma once
#include "appframe.h"
#include <string>

class SoundServer;

class SoundItemBase
{
public:
	enum class TYPE {
		BGM = 0,	// ストリーミング。ループ再生
		SE,			// １回のみ再生
		VOICE,		// ストリーミング。１回のみ再生
		ONESHOT,	// ストリーミング。１回のみ再生。即時再生し、停止したらサーバから削除
	};
	SoundItemBase(std::string filename);
	virtual ~SoundItemBase();
	void SetSoundServer(SoundServer* sndServer) { _sndServer = sndServer; }

	virtual void	Unload();
	virtual bool	IsLoad();
	virtual void	Play() = 0;
	virtual bool	IsPlay();
	virtual void	Stop();

	virtual int		GetVolume();
	virtual void	SetVolume(int volume);
	virtual int		GetPan();
	virtual void	SetPan(int pan);
	virtual int		GetFrequency();
	virtual void	SetFrequency(int frequency);
	virtual void	ResetFrequency();

	virtual TYPE	GetType() = 0;
	virtual std::string GetFileName() { return _filename; }
	virtual int		GetSoundHandle() { return _snd; }

	virtual void	Update() {}

protected:
	virtual void	PlayMem(int flg);

	int		_snd;
	std::string	_filename;
	int		_volume;
	int		_pan;
	int		_frequency;
	SoundServer* _sndServer;
}; 

class SoundItemStatic : public SoundItemBase
{
	typedef	SoundItemBase	base;
public:
	SoundItemStatic(std::string filename) : base(filename) {
		SetCreateSoundDataType(DX_SOUNDDATATYPE_MEMNOPRESS);
		_snd = LoadSoundMem(_filename.c_str());
		if (_frequency == 0) {
			_frequency = GetFrequencySoundMem(_snd);
		}
	}
	virtual ~SoundItemStatic() {}
};

class SoundItemSE : public SoundItemStatic
{
	typedef	SoundItemStatic	base;
public:
	SoundItemSE(std::string filename) : base(filename) {
	}
	virtual TYPE	GetType() { return TYPE::SE; }
	virtual void	Play() {
		Stop();		// SEは一度止めてから
		PlayMem(DX_PLAYTYPE_BACK);
	}
};



class SoundItemStream : public SoundItemBase
{
	typedef	SoundItemBase	base;
public:
	SoundItemStream(std::string filename) : base(filename) {
	}
	virtual ~SoundItemStream() {}
	virtual bool	IsLoad() { return true; }	// ストリーム再生のものは、ロードされているものとする
protected:
	void	StreamLoad() {
		if (_snd == -1) {
			SetCreateSoundDataType(DX_SOUNDDATATYPE_FILE);
			_snd = LoadSoundMem(_filename.c_str());
			if (_frequency == 0) {
				_frequency = GetFrequencySoundMem(_snd);
			}
		}
	}
	virtual void	Update() {
		if (_snd != -1) {
			if (IsPlay() == false) {
				Unload();		// ストリーム再生のものは、再生していなかったらメモリからアンロードする
			}
		}
	}
};

class SoundItemBGM : public SoundItemStream
{
	typedef	SoundItemStream	base;
public:
	SoundItemBGM(std::string filename) : base(filename) {
	}
	virtual TYPE	GetType() { return TYPE::BGM; }
	virtual void	Play();
};

class SoundItemVOICE : public SoundItemStream
{
	typedef	SoundItemStream	base;
public:
	SoundItemVOICE(std::string filename) : base(filename) {
	}
	virtual TYPE	GetType() { return TYPE::VOICE; }
	virtual void	Play() {
		StreamLoad();	// ストリーム再生のものをロードする
		if (IsPlay() == false) {	// VOICEは再生されていない場合のみ
			PlayMem(DX_PLAYTYPE_BACK);
		}
	}
};

class SoundItemOneShot : public SoundItemStream
{
	typedef	SoundItemStream	base;
public:
	SoundItemOneShot(std::string filename) : base(filename) {
	}
	virtual TYPE	GetType() { return TYPE::ONESHOT; }
	virtual void	Play() {
		StreamLoad();	// ストリーム再生のものをロードする
		PlayMem(DX_PLAYTYPE_BACK);
	}
	virtual void	Update();

};
