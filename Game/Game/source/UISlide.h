#pragma once

#include "appframe.h"
#include "UIBase.h"

class UISlide : public UIBase {
public:
	UISlide(float initPosX, float initPosY, float endPosX, float endPosY, int frame);

public:
	void Process()override;

public:
	int Selected()override;

};


