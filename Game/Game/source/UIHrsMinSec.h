#pragma once
#include"UI.h"
#include<array>
#include"appframe.h"
class UIHrsMinSec :public UI {
public:
	UIHrsMinSec(int frmae,int secondTime,const Transform2& transform,const Vector2& size);
	~UIHrsMinSec();

	const char* GetName()override { return "HrsMinSec"; }

	bool Process()override;
	bool Draw()override;

	void SetSecondTime(int secondTime) { _secondTime = secondTime; }

	void SetIsDraw(bool isDraw) { _isDraw = isDraw; }
private:
	std::array<class SpriteNumber*, 3>_spriteTimes;
	class SpriteText* _spriteColon;

	int _secondTime;		//秒数

	//イージング用
	int _frameCnt;			//フレームカウント
	const int _frame;		//イージングにかけるフレーム数

	bool _isDraw;			//描画するかどうか
};