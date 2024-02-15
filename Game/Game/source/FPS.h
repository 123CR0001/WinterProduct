#pragma once

#include"appFrame.h"

class FPS {
public:
	FPS();
	~FPS();

	bool Update();
	void WaitFPS() const;
	void SetFPS(float fps);

public:
	float _startTime;         //測定開始時刻
	int _count;             //カウンタ
	float _fps;             //fps
	int _cntAverage;//平均を取るサンプル数
	int _configuredFPS;	//設定したFPS
};

