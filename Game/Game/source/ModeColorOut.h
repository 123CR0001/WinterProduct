#pragma once
#include "appframe.h"
#include<functional>

class ModeColorOut :public ModeBase {
public:
	//フェードインで使用するモード、
	ModeColorOut(class ModeColorIn* modeColorIn, std::function<void()>func,int maxCnt = 30);
	bool Initialize()override;
	bool Terminate()override;
	bool Process()override;
	bool Render()override;

protected:

	int _frameCnt;
	const int _frameMaxCnt;

	int _alpha;

	int _red;
	int _green;
	int _blue;

	class ModeColorIn* _modeColorIn;

	std::function<void()>_func;
};