#pragma once

#include "appframe.h"

class ModeGameUI : public ModeBase {
		typedef ModeBase base;
public:
	ModeGameUI();
	~ModeGameUI();

	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();

public:
	class UIServer* _uiServer;
	class UILightsTimer* _uiLightsTimer;
};

