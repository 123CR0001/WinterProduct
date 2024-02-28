#pragma once

#include "appframe.h"
#include "UIBase.h"

class UIDetection : public UIBase {
public:
	 UIDetection(class ModeGame* game);

public:
	void Process();
	void Render()override;

protected:
	class ModeGame* _game;

	struct ENEMY_INFORMATION {
		bool bFind;
		int val;
	};
	ENEMY_INFORMATION _eneInfo;

	std::vector<ENEMY_INFORMATION> _vEneInfo;
};

