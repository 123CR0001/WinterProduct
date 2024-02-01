#pragma once
#include "appframe.h"

class ModeColorOut :public ModeBase {
public:
	ModeColorOut(class ModeColorIn* modeColorIn,ModeBase* nowMode = nullptr, int maxCnt = 30, ModeBase* nextMode = nullptr, int layer = 0 , const char* name = nullptr);
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
	ModeBase* _nextMode;
	int _nextModeLayer;
	const char* _nextModeName;
	ModeBase* _nowMode;
	class ModeColorIn* _modeColorIn;
};