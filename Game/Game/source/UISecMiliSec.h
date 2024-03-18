#pragma once
#include"UI.h"
#include<memory>
#include"appframe.h"

class UISecMiliSec :public UI {
public:
	UISecMiliSec(const Transform2& transform, int drawOrder = 100);
	~UISecMiliSec();

	const char* GetName()override { return "SecMiliSec"; }

	bool Process()override;
	bool Draw()override;

	void SetFrameCount(int frameCnt) { _frameCnt = frameCnt; }

	void SetIsDraw(bool isDraw) { _isDraw = isDraw; }

private:
	class std::unique_ptr<class SpriteNumber> _sec;
	class std::unique_ptr<class SpriteNumber> _miliSec;
	class std::unique_ptr<class SpriteText>_colon;

	int _frameCnt;

	Vector2 _pos;

	bool _isDraw;
};