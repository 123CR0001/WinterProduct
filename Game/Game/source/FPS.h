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
	float _startTime;         //����J�n����
	int _count;             //�J�E���^
	float _fps;             //fps
	int _cntAverage;//���ς����T���v����
	int _configuredFPS;	//�ݒ肵��FPS
};

