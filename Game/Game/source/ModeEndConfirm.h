#pragma once

#include "appframe.h"

class ModeEndConfirm : public ModeBase
{
	typedef ModeBase base;
	typedef ResourceServer res;

public:
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();

private:
	// UIProcessクラス
	class		UIServer* _ui;
	// 選択肢の総数
	const int	MAX_OPTIONS = 2;
	// UIのレイヤー値
	// value：10
	const int LOW_LAYER_VALUE = 40;
	// value：20
	const int BASIC_LAYER_VALUE = 50;
	// value：30
	const int HIGH_LAYER_VALUE = 60;

	// 何を選択しているか
	int			_select;
};

