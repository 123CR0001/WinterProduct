#include "SoundServer.h"
#include "SoundItem.h"

SoundServer::SoundServer() {
	_isUpdate = false;
	_cntOneShot = 0;
}
SoundServer::~SoundServer() {
	Clear();
}

void SoundServer::Clear() {
	for (auto&& e : _v) {
		delete e.second;
	}
	_v.clear();
}

void SoundServer::Add(SoundItemOneShot* snd) {
	std::string name = "__ONESHOT_SOUND_" + std::to_string(_cntOneShot);
	_cntOneShot++;
	Add(name, snd);
	snd->Play();		// OneShotは登録した瞬間再生する
}

void SoundServer::Add(std::string name, SoundItemBase* snd) {
	if (_isUpdate) {
		_vAdd[name] = snd;
	}
	else {
		_v[name] = snd;
	}
	snd->SetSoundServer(this);
}

bool SoundServer::Del(SoundItemBase* snd) {
	for (auto&& e : _v) {
		if (e.second == snd) {
			if (_isUpdate) {
				_vDel[e.first] = e.second;
			}
			else {
				std::string name = e.first;
				delete _v[name];
				_v.erase(name);
			}
			return true;
		}
	}
	return false;
}

bool SoundServer::Del(std::string name) {
	if (_v.count(name) > 0) {
		return Del(_v[name]);
	}
	return false;
}

SoundItemBase* SoundServer::Get(std::string name) {
	if (_v.count(name) > 0) { return _v[name]; }
	return NULL;
}

void SoundServer::Play(std::string name) {
	auto soundItem = Get(name);
	if(soundItem)soundItem->Play();
}

void SoundServer::StopType(SoundItemBase::TYPE type) {
	for (auto&& e : _v) {
		if (e.second->GetType() == type) {
			e.second->Stop();
		}
	}
}

void SoundServer::Update() {
	// 追加リストのものを追加
	for (auto&& e : _vAdd) { Add(e.first, e.second); }
	_vAdd.clear();

	// Updateを回す
	_isUpdate = true;
	for (auto&& e : _v) {
		e.second->Update();
	}
	_isUpdate = false;

	// 削除リストのものを削除
	for (auto&& e : _vDel) { Del(e.second); }
	_vDel.clear();
}
