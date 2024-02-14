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

	// �I�����̑���
	int	_maxOptions;
	const int STAGE_SELECT_NUM = 4;
	const int AREA_SELECT_NUM = 5;
	// UI�̃��C���[�l
	// value�F10
	const int LOW_LAYER_VALUE = 40;
	// value�F20
	const int BASIC_LAYER_VALUE = 50;
	// value�F30
	const int HIGH_LAYER_VALUE = 60;

	// ����I�����Ă��邩
	int	_select;
	bool _bSelected;
};

