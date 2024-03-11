#pragma once
#include"UI.h"
#include<array>
#include"appframe.h"
class UIHrsMinSec :public UI {
public:
	UIHrsMinSec(int frmae,int secondTime,const Transform2& transform,const Vector2& size);
	~UIHrsMinSec();

	bool Process()override;
	bool Draw()override;

	void SetSecondTime(int secondTime) { _secondTime = secondTime; }

	void SetIsDraw(bool isDraw) { _isDraw = isDraw; }
private:
	std::array<class SpriteNumber*, 3>_spriteTimes;
	class SpriteText* _spriteColon;

	//
	int _secondTime;

	//イージング用
	int _frameCnt;
	const int _frame;

	bool _isDraw;
};