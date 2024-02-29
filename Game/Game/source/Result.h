#pragma once

#include "appframe.h"

class Result {

protected:
	int _startTime;
	int _clearTime;
	float _maxDetection;
	int _continuousKills;
	int _diedTime;
	bool _bDeath;

	enum class ClearTimeRank {
		ksrank = 5000,
		karank = 10000,
		kbrank = 15000,
		kcrank = 20000
	};
	ClearTimeRank _eTimeRank;

	enum class MaxDetectionRank : int {
		kSrank = 25,
		kArank = 50,
		kBrank = 75,
		kCrank = 100
	};
	MaxDetectionRank _eDetectionRank;

private:
	class ModeGame* _game;
	class ObjectServer* _objectServer;

public:
	Result();
	~Result();

	bool Process();
	bool Render();

public:
	void SetStartTime(int startTime) { _startTime = startTime; }
	void SetClearTime(int clearTime) { _clearTime = clearTime; }
	void SetMaxDetection(float maxDetection) { _maxDetection = maxDetection; }
	void SetContinuousKills(int continuousKills) { _continuousKills = continuousKills; }
	int GetClearTime() { return _clearTime; }
	float GetMaxDetection() { return _maxDetection; }
	int GetContinuousKills() { return _continuousKills; }
	ClearTimeRank GetTimeRank() { return _eTimeRank; }
	MaxDetectionRank GetDetectionRank() { return _eDetectionRank; }
	void SetModeGame(class ModeGame* game) { _game = game; }
	void SetObjectServer(class ObjectServer* objectServer) { _objectServer = objectServer; }


public:
	bool EvaluateClearTime(int clearTime);
	bool TimeRankProcess();
	bool TimeRankRender();
	bool EvaluateMaxDetection(float maxDetection);
	bool DetectionRankProcess();
	bool DetectionRankRender();
	bool ContinuousKillsProcess();
	bool ContinuousKillsRender();
};

