#pragma once

#include "appframe.h"
#include "UIBase.h"
class UIBlink : public UIBase {
public:
	UIBlink();

public:
	void Process()override;
	void Render()override;

protected:
	int _alphaVal;
	int _blinkFrame;
	int _blinkCnt;
	const int ALPHA_VAL_LOWLIMIT = 100;
	const int ALPHA_VAL_HIGHLIMIT = 250;
	enum class STATE {
		kUp,
		kDown
	}_state;
};

