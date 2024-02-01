#pragma once

#include "appframe.h"

class ModePause : public ModeBase {
	typedef ModeBase base;
	typedef ResourceServer res;

public:
	bool Initialize()override;
	bool Terminate()override;
	bool Process()override;
	bool Render()override;

	void SelectedItemProcess(int selectResult);
	void AdjustItemPos(std::string selectedItem);
	void PoseEndProcess();
	bool AllMoveFinished();

	int GetSelect();

protected:
	class UIServer* _ui;

	// valueÅF10
	const int LOW_LAYER_VALUE	= 10;
	// valueÅF20
	const int BASIC_LAYER_VALUE = 20;
	// valueÅF30
	const int HIGH_LAYER_VALUE	= 30;

	const int MAX_OPTIONS = 4;
	int	_select;
	bool _bToGeme;
	bool _bToGameFade;

};

