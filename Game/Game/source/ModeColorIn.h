#pragma once
#include"appframe.h"

class ModeColorIn :public ModeBase {
public:
	ModeColorIn(int _maxCnt = 30, bool isBlack = false);
	bool Initialize()override;
	bool Terminate()override;
	bool Process()override;
	bool Render()override;
protected:
	int _frameCnt;
	int _frameMaxCnt;
	int _alpha;
	int _red;
	int _green;
	int _blue;
	bool _isBlack;
};