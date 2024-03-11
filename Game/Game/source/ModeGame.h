#pragma once
#include "appframe.h"
#include<memory>

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


	XGamePad* GetPad()const;

	class ObjectServer* GetObjectServer()const { return _objServer; }
	class ModeEffekseer* GetModeEffekseer()const { return _modeEffekseer; }
	class FPS* GetFps()const { return _fps; }
	std::string GetStage()const { return _stage; }

	void IncrementEnergyCount() { _energyCount++; }
	void DecremetEnergyCOunt() { _energyCount--; }
	void IncrementEnemyCount() { _enemyCount++; }
	void DecrementEnemyCount() { _enemyCount--; }

	class TimeLine* GetTimeLine()const { return _timeLine; }

	class MyUIServer* GetUIServer()const { return _uiServer; }

	void SwitchOverOrClear();
protected:

	class ObjectServer* _objServer;
	class ModeEffekseer* _modeEffekseer;

	int _handleShadowMap;

	//LightsOutを使えるのはこの変数が０の時
	int _energyCount;

	//倒す敵の数
	int _enemyCount;

	class LightsOut* _lightsOut;

	//
	class TimeLine* _timeLine;

	class MyUIServer* _uiServer;

	class FPS* _fps;

	int _bg;

	std::string _stage;

	//デバッグ用
	class ModeDebugMenu* _debug;
public:
	class ModeDebugMenu* GetDebugMenu() { return _debug; }

	class ResultData {
	public:
		ResultData() {
			maxCombo = 0;
			clearSecondTime = 0.f;
			maxDetectionLevel = 0.f;
		}
		~ResultData(){}
		int maxCombo;
		float clearSecondTime;
		float maxDetectionLevel;
	};

	std::shared_ptr<ResultData> GetResultData() const { return _resultData; }
private:
	std::shared_ptr<ResultData>_resultData;
}; 
