#pragma once
#include "appframe.h"
#include "ModeGame.h"

class ModeClear :public ModeBase {
	typedef ModeBase base;
	typedef ResourceServer res;
public:
	ModeClear(std::shared_ptr<ModeGame::ResultData> data);

	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();

private:

	void SetUI();
	void SetButton();

	std::shared_ptr<ModeGame::ResultData> _resultData;

	class ButtonServer* _buttonServer;

	//UI
	class MyUIServer* _uiServer;

	class TimeLine* _timeLine;
};