#pragma once

#include "appframe.h"
#include "UISlide.h"

class UIGetSlide : public UISlide
{
public:
	UIGetSlide(float initPosX, float initPosY, float endPosX, float endPosY, int frame);

	void SetSelecting(int select);
	int GetSelecting();

protected:
	int _selecting;
};

