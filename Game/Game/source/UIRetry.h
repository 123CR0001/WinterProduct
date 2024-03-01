#pragma once
#include "ModeGameOver.h"

#include "appframe.h"
#include "UIBase.h"

class UIRetry : public UIBase {
public:
	UIRetry(ModeGameOver* over);

public:
	int Selected();

protected:
	ModeGameOver* _over;	// GameOver‚Ìƒ|ƒCƒ“ƒ^
};

