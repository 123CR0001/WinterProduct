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
	int _frameCnt;			//フレームカウント
	int _frameMaxCnt;		//最大フレームカウント
	int _alpha;				//アルファ値
	int _red;				//赤
	int _green;				//緑
	int _blue;				//青
	bool _isBlack;			//黒かどうか
};