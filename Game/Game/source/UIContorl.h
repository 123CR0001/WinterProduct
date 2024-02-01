#pragma once

#include "appframe.h"
#include "UISlide.h"

class UIContorl : public UISlide {
public:
	UIContorl(float initPosX, float initPosY, float endPosX, float endPosY, int frame);

public:
	void Render()override;

public:
	int Selected()override;

};

