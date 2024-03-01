#pragma once
#include"Siren.h"

class MiniSiren : public Siren {
public:
	MiniSiren(class ObjectServer* server) :Siren(server){
		_SEName = "SE_00";
		_volumeSize = 300.f;
	}
	~MiniSiren(){}
};