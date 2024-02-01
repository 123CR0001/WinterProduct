#pragma once

#include "UIBase.h"

class UIGameStart : public UIBase
{
public:
	UIGameStart();

	virtual int Selected();

protected:
	class ModeTitle* mdTitle;
};

