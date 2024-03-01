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

	const int MAX_OPTIONS = 2;	// 項目の最大数
	const int LOW_LAYER_VALUE = 10;		// レイヤー値（10）
	const int BASIC_LAYER_VALUE = 20;	// レイヤー値（20）
	const int HIGH_LAYER_VALUE = 30;	// レイヤー値（30）

	int _select;	// 選択中の項目


};