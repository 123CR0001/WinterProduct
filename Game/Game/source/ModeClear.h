#pragma once
#include "appframe.h"

class ModeClear :public ModeBase {
	typedef ModeBase base;
public:
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();

protected:

	int _cg;

};