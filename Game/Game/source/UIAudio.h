#pragma once

#include "appframe.h"
#include "UISlide.h"

class UIAudio : public UISlide {
public:
	UIAudio(float initPosX, float initPosY, float endPosX, float endPosY, int frame);

public:
	int Selected()override;
};

