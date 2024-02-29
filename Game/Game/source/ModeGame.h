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
	int GetCnt()const { return _cntTest; }	// �������@��ŏ���
	std::string GetStage()const { return _stage; }
protected:

	class ObjectServer* _objServer;
	class ModeEffekseer* _modeEffekseer;

	//���C�c�A�E�g����x�ł��g�p�������́A���ʂɎg�p
	bool _isCouldLightsOut;

	int _handleShadowMap;

	class FPS* _fps;

	//�f�o�b�O�p
	class ModeDebugMenu* _debug;
public:
	class ModeDebugMenu* GetDebugMenu() { return _debug; }

	class UIServer* _uiServer;

	std::string _stage;
	class Timer* _timer;
	int _cntTest;	// �������@��ŏ���
}; 
