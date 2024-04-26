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
	int GetEnemyCount()const { return _enemyCount; }

	class TimeLine* GetTimeLine()const { return _timeLine; }

	class UIScreen* GetUIServer()const { return _UIScreen; }

	void SetIsUseLightsOut(bool isUseLightsOut) { _isUsingLightsOut = isUseLightsOut; }
	bool IsUsingLightsOut()const { return _isUsingLightsOut; }

	void SwitchOverOrClear();
protected:

	class ObjectServer* _objServer;
	class ModeEffekseer* _modeEffekseer;

	int _handleShadowMap;

	//LightsOutを使えるのはこの変数が０の時
	int _energyCount;

	//倒す敵の数
	int _enemyCount;

	//ライツアウトが使用中か
	bool _isUsingLightsOut;

	//タイムライン
	class TimeLine* _timeLine;

	//UI
	class UIScreen* _UIScreen;

	//_enemyCountを表示するUI
	class SpriteText* _enemyCountBg;

	//_energyCountを表示するUI
	class SpriteNumber* _enemyCountText;

	//背景
	int _bg;

	//ステージ番号
	std::string _stage;

public:
	std::shared_ptr<class ResultData> GetResultData() const { return _resultData; }
private:
	std::shared_ptr<class ResultData>_resultData;

}; 
