#pragma once
#include "appframe.h"

class ModeMiniMap :public ModeBase {
public:
	ModeMiniMap(class ModeGame* game);
	~ModeMiniMap();

	bool Initialize()override;
	bool Process()override;
	bool Render()override;

private:
	class ModeGame* _game;
};