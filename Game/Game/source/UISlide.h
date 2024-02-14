#pragma once

#include "appframe.h"
#include "UIBase.h"

class UISlide : public UIBase {
public:
	UISlide(float initPosX, float initPosY, float endPosX, float endPosY, int frame);

public:
	virtual void Process();

public:
	int Selected()override;

};


