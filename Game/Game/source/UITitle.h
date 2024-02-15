#pragma once

#include "appframe.h"
#include "UISlide.h"

class UITitle : public UISlide {
public:
	UITitle(float initPosX, float initPosY, float endPosX, float endPosY, int frame, std::string nowMode);

public:
	int Selected()override;

protected:
	std::string _nowMode;
};

