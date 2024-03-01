#pragma once
#include "appframe.h"

class ModeGame : public ModeBase
{
	typedef ModeBase base;
public:
	ModeGame(std::string stageNum);
	~ModeGame();

	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();

	bool LoadData();

	XGamePad* GetPad()const;

	class ObjectServer* GetObjectServer()const { return _objServer; }
	class ModeEffekseer* GetModeEffekseer()const { return _modeEffekseer; }
	class FPS* GetFps()const { return _fps; }
	int GetCnt()const { return _cntTest; }	// お試し　後で消す
	std::string GetStage()const { return _stage; }
protected:

	class ObjectServer* _objServer;
	class ModeEffekseer* _modeEffekseer;

	//ライツアウトを一度でも使用したかの、判別に使用
	bool _isCouldLightsOut;

	int _handleShadowMap;

	class FPS* _fps;

	//デバッグ用
	class ModeDebugMenu* _debug;
public:
	class ModeDebugMenu* GetDebugMenu() { return _debug; }

	class UIServer* _uiServer;

	std::string _stage;
	class Timer* _timer;
	int _cntTest;	// お試し　後で消す
}; 
