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

	//LightsOut‚ğg‚¦‚é‚Ì‚Í‚±‚Ì•Ï”‚ª‚O‚Ì
	int _energyCount;

	//“|‚·“G‚Ì”
	int _enemyCount;

	//ƒ‰ƒCƒcƒAƒEƒg‚ÌƒNƒ‰ƒX
	class LightsOut* _lightsOut;

	//ƒ^ƒCƒ€ƒ‰ƒCƒ“
	class TimeLine* _timeLine;

	//UI
	class MyUIServer* _uiServer;

	class SpriteText* _enemyCountBg;
	class SpriteNumber* _enemyCountText;

	class FPS* _fps;

	int _bg;

	std::string _stage;

	//ƒfƒoƒbƒO—p
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
			//ŠÔ
			if(clearSecondTime < 60.f * 1.f) {			//•]‰¿S
				point += 3;
			}
			else if(clearSecondTime < 60.f * 2.f) {	//•]‰¿A
				point += 2;
			}
			else if(clearSecondTime < 60.f * 3.f) {	//•]‰¿B
				point += 1;
			}
			else {										//•]‰¿C
				point += 0;
			}

			//
			if(maxDetectionLevel < 0.25f) {				//•]‰¿S
				point += 3;
			}
			else if(maxDetectionLevel < 0.5f) {			//•]‰¿A
				point += 2;
			}
			else if(maxDetectionLevel < 0.75f) {		//•]‰¿B
				point += 1;
			}
			else {										//•]‰¿C
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
