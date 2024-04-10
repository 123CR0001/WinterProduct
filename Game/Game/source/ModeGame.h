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

	class MyUIServer* GetUIServer()const { return _uiServer; }

	class LightsOut* GetLightsOut()const { return _lightsOut; }

	void SwitchOverOrClear();
protected:

	class ObjectServer* _objServer;
	class ModeEffekseer* _modeEffekseer;

	int _handleShadowMap;

	//LightsOut���g����̂͂��̕ϐ����O�̎�
	int _energyCount;

	//�|���G�̐�
	int _enemyCount;

	//���C�c�A�E�g�̃N���X
	class LightsOut* _lightsOut;

	//�^�C�����C��
	class TimeLine* _timeLine;

	//UI
	class MyUIServer* _uiServer;

	//_enemyCount��\������UI
	class SpriteText* _enemyCountBg;

	//_energyCount��\������UI
	class SpriteNumber* _enemyCountText;

	//�w�i
	int _bg;

	//�X�e�[�W�ԍ�
	std::string _stage;

public:
	std::shared_ptr<class ResultData> GetResultData() const { return _resultData; }
private:
	std::shared_ptr<class ResultData>_resultData;

	int _pTime;
}; 
