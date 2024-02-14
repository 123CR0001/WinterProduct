#pragma once

#include "appframe.h"
#include "UISlide.h"

class UISelectController : public UISlide {
	typedef ResourceServer res;
public:
	UISelectController(float initPosX, float initPosY, float endPosX, float endPosY, int frame);

public:
	int Selected()override;

protected:
	// valueÅF10
	const int LOW_LAYER_VALUE = 40;
	// valueÅF20
	const int BASIC_LAYER_VALUE = 50;
	// valueÅF30
	const int HIGH_LAYER_VALUE = 60;
	bool bSelected;
};

