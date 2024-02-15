#pragma once

#include "appframe.h"
#include "UISlide.h"

class UISelectMap : public UISlide {
public:
	UISelectMap(float initPosX, float initPosY, float endPosX, float endPosY, int frame, std::string stage);

public:
	int Selected()override;

protected:
	std::string _stageNum;
};

