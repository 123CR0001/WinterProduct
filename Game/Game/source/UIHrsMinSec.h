#pragma once
#include"UI.h"
#include<array>

class UIHrsMinSec :public UI {
public:
	UIHrsMinSec(int frmaeCnt,int time);
	~UIHrsMinSec();

	bool Process()override;
	bool Draw()override;

private:
	std::array<class SpriteNumber*, 3>_spriteTimes;
	class SpriteText* _spriteColon;

	//
	int _secondTime;

	//イージング用
	int _frameCnt;
	const int _frame;
};