#pragma once
#include "appframe.h"

class ModeGame : public ModeBase
{
	typedef ModeBase base;
public:
	ModeGame(std::string stageNum);

	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();

	bool LoadData();

	XGamePad* GetPad()const;

	class ObjectServer* GetObjectServer()const { return _objServer; }
	class ModeEffekseer* GetModeEffekseer()const { return _modeEffekseer; }
	class FPS* GetFps()const { return _fps; }
	int GetCnt()const { return _cntTest; }	// Ç®ééÇµÅ@å„Ç≈è¡Ç∑
protected:

	class ObjectServer* _objServer;
	class ModeEffekseer* _modeEffekseer;
	class UIServer* _uiServer;
	class FPS* _fps;
	std::string _stage;
	class Timer* _timer;
	int _cntTest;	// Ç®ééÇµÅ@å„Ç≈è¡Ç∑
}; 
