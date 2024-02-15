#pragma once

#include "appframe.h"
#include "UISlide.h"

class UIToConfirm : public UISlide {
public:
	UIToConfirm(float initPosX, float initPosY, float endPosX, float endPosY, int frame);

public:
	int Selected()override;
};

