#pragma once

#include "appframe.h"
#include "UIBase.h"

class UINextStage : public UIBase {
public:
	UINextStage(std::string stage);

public:
	int Selected();

protected:
	std::string _stage;
};

