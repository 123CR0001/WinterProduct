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

	int _frameCnt;				//フレームカウント
	const int _frameMaxCnt;		//最大フレームカウント

	int _alpha;					//アルファ値

	int _red;					//赤
	int _green;					//緑		
	int _blue;					//青

	class ModeColorIn* _modeColorIn;		//フェードインで使用するモード

	std::function<void()>_func;			//フェードアウト後に呼び出す関数
};