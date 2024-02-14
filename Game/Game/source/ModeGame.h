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

	//���C�c�A�E�g����x�ł��g�p�������́A���ʂɎg�p
	bool _isCouldLightsOut;

	int number;
}; 
