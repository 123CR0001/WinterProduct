#pragma once

#include "UIBase.h"
#include "UISlide.h"

class UIRemainingUses : public UISlide {
	typedef UISlide slide;
	typedef ResourceServer res;
public:
	UIRemainingUses(class ModeGame* game, float initPosX, float initPosY, float endPosX, float endPosY, int frame);

public:
	void Terminate()override;
	void Render()override;

public:
	int Selected()override;

protected:
#define REMAIN_NUM 10
	// ‰æ‘œ
	std::map<std::string, std::vector<int>> _mCg;

private:
	class ModeGame* _game;

};

