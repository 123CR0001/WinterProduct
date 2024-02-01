#pragma once

#include "appframe.h"
#include "UISlide.h"

class UIAudio : public UISlide {
public:
	UIAudio(float initPosX, float initPosY, float endPosX, float endPosY, int frame);

	void Terminate()override;
	void Render()override;

public:
	int Selected()override;
};

