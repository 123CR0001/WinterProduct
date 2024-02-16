#pragma once

#include "appframe.h"

class Timer {
	public:
	Timer();
	~Timer();

	bool Process();

public:
	bool Start();
	bool Stop();
	bool Reset();
	std::string CheckTimeUnit();
	bool AddS(int second);
	bool AddMS(int mSecond);

	void SetTimeS(int second) { _setTimeS = second; }
	void SetTimeMS(int mSecond) { _setTimeMS = mSecond; }
	int GetTimeS()const { return _second; }
	int GetTimeMS()const { return _mSecond; }

protected:
	int _setTimeS;
	int _setTimeMS;
	int _second;
	int _mSecond;
	int _stopTime;
	bool _bStart;

	int _dxStartCnt;
};

