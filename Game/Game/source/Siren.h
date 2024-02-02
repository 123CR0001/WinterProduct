#pragma once
#include "ObjectBase.h"

class Siren :public ObjectBase {
public:
	Siren(class ObjectServer* server);
	~Siren();
	
	bool Initialize()override;
	bool Terminate()override;
	bool Process()override;

	int GetInterval()const { return _interval; }
	
private:
	int _interval;

	static int _maxInterval;
	static float _playerDist;
	static float _volumeSize;
};