#pragma once

#include "appframe.h"

class ModeSelect : public ModeBase {
	typedef ModeBase base;
	typedef ResourceServer res;

public:
	bool Initialize()override;
	bool Terminate()override;
	bool Process()override;
	bool Render()override;

	void Load();
	bool SwitchTagWithBg(std::string);
	bool AdjustItemPos(std::string selectedItem);

protected:
	class UIServer* _ui;

	// 選択肢の総数
	int	_maxOptions;
	const int STAGE_SELECT_NUM = 4;
	const int AREA_SELECT_NUM = 5;
	// UIのレイヤー値
	// value：10
	const int LOW_LAYER_VALUE = 40;
	// value：20
	const int BASIC_LAYER_VALUE = 50;
	// value：30
	const int HIGH_LAYER_VALUE = 60;

	// 何を選択しているか
	int	_select;
	bool _bSelected;
};

