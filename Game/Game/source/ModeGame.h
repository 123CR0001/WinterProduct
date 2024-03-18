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
	std::string GetStage()const { return _stage; }

	void IncrementEnergyCount() { _energyCount++; }
	void DecremetEnergyCOunt() { _energyCount--; }
	int GetEnergyCount()const { return _energyCount; }

	void IncrementEnemyCount() { _enemyCount++; }
	void DecrementEnemyCount();

	class TimeLine* GetTimeLine()const { return _timeLine; }

	class MyUIServer* GetUIServer()const { return _uiServer; }

	class LightsOut* GetLightsOut()const { return _lightsOut; }

	void SwitchOverOrClear();
protected:

	class ObjectServer* _objServer;
	class ModeEffekseer* _modeEffekseer;

	int _handleShadowMap;

	//LightsOutを使えるのはこの変数が０の時
	int _energyCount;

	//倒す敵の数
	int _enemyCount;

	//ライツアウトのクラス
	class LightsOut* _lightsOut;

	//タイムライン
	class TimeLine* _timeLine;

	//UI
	class MyUIServer* _uiServer;

	class SpriteText* _enemyCountBg;
	class SpriteNumber* _enemyCountText;

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
		std::string _stageName;

		const char* GetRank() {
			int point = 0;
			//時間
			if(clearSecondTime < 60.f * 1.f) {			//評価S
				point += 3;
			}
			else if(clearSecondTime < 60.f * 2.f) {	//評価A
				point += 2;
			}
			else if(clearSecondTime < 60.f * 3.f) {	//評価B
				point += 1;
			}
			else {										//評価C
				point += 0;
			}

			//
			if(maxDetectionLevel < 0.25f) {				//評価S
				point += 3;
			}
			else if(maxDetectionLevel < 0.5f) {			//評価A
				point += 2;
			}
			else if(maxDetectionLevel < 0.75f) {		//評価B
				point += 1;
			}
			else {										//評価C
				point += 0;
			}

			const char* rank = nullptr;

			if(point <= 1) {
				rank = "c";
			}
			else if(point <= 3) {
				rank = "b";
			}
			else if(point <= 5) {
				rank = "a";
			}
			else {
				rank = "s";
			}

			return rank;
		}
	};

	std::shared_ptr<ResultData> GetResultData() const { return _resultData; }
private:
	std::shared_ptr<ResultData>_resultData;

	int _pTime;
}; 
