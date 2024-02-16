#pragma once

#include "UIBase.h"

class UILightsTimer : public UIBase {
	typedef ResourceServer res;
public:
	UILightsTimer();
	~UILightsTimer();

	void Process()override;
	void Render()override;	

public:
	int Selected()override;
	bool OpenFrame();
	void AnimProcess();

protected:
	const int FRAME_NUM = 10;
	const int TIME_NUM = 20;

	std::map<std::string, std::vector<int>> _mCg;
	std::map<std::string, int> _mDigit;
	class Timer* _timer;
	bool _bFrameOpen;
	int _animCnt;
	int _frameCnt;
	int _motCnt;
};

