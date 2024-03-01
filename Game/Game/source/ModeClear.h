#pragma once
#include "appframe.h"
#include "ModeGame.h"

class ModeClear :public ModeBase {
	typedef ModeBase base;
	typedef ResourceServer res;
public:
	ModeClear(ModeGame* game);

	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();

public:
	void Load();

protected:
#define STR(var) #var

	class ModeGame* _game;
	class UIServer* _uiServer;
	std::string _stage;

	const int MAX_OPTIONS = 2;	// ���ڂ̍ő吔
	const int LOW_LAYER_VALUE = 10;		// ���C���[�l�i10�j
	const int BASIC_LAYER_VALUE = 20;	// ���C���[�l�i20�j
	const int HIGH_LAYER_VALUE = 30;	// ���C���[�l�i30�j

	int _select;	// �I�𒆂̍���


};