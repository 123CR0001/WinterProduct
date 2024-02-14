#pragma once
#include "appframe.h"

class ModeGame : public ModeBase
{
	typedef ModeBase base;
public:
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();

	bool LoadData();

	XGamePad* GetPad()const;

	class ObjectServer* GetObjectServer()const { return _objServer; }
	class ModeEffekseer* GetModeEffekseer()const { return _modeEffekseer; }
protected:

	class ObjectServer* _objServer;
	class ModeEffekseer* _modeEffekseer;

	//ライツアウトを一度でも使用したかの、判別に使用
	bool _isCouldLightsOut;

	int number;
}; 
