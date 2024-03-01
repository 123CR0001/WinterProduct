#pragma once
#include "ObjectBase.h"

class Siren :public ObjectBase {
public:
	Siren(class ObjectServer* server);
	virtual ~Siren();
	
	bool Initialize()override;
	bool Terminate()override;
	bool Process()override;

	int GetInterval()const { return _interval; }
	
protected:
	int _interval;
	const char* _SEName;
	float _volumeSize;
private:
	static int _maxInterval;
	static float _playerDist;
};