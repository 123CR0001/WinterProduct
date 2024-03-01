#include "Result.h"
#include "ModeGame.h"

#include "ObjectServer.h"
#include "CommonSoldier.h"
#include "AIComponent.h"
#include "AIState.h"

Result::Result() {
	_startTime = 0;
	_clearTime = 0;
	_maxDetection = 0;
	_continuousKills = 0;
	_diedTime = 0;
	_bDeath = false;

	_eTimeRank = ClearTimeRank::ksrank;
	_eDetectionRank = MaxDetectionRank::kSrank;

	_game = nullptr;
	_objectServer = nullptr;
}

Result::~Result() {

}

bool Result::Process() {

	TimeRankProcess();
	DetectionRankProcess();
	ContinuousKillsProcess();

	EvaluateClearTime(_clearTime);
	EvaluateMaxDetection(_maxDetection);

	return true;
}

bool Result::Render() {

	TimeRankRender();
	//DetectionRankRender();
	//ContinuousKillsRender();
	return true;
}

bool Result::EvaluateClearTime(int clearTime) {
	if (clearTime == 0) { return false; }
	else if (clearTime <= static_cast<int>(ClearTimeRank::ksrank)) { _eTimeRank = ClearTimeRank::ksrank; }
	else if (clearTime <= static_cast<int>(ClearTimeRank::karank)) { _eTimeRank = ClearTimeRank::karank; }
	else if (clearTime <= static_cast<int>(ClearTimeRank::kbrank)) { _eTimeRank = ClearTimeRank::kbrank; }
	else { _eTimeRank = ClearTimeRank::kcrank; }
	return true;
}

bool Result::TimeRankProcess() {
	if (_game == nullptr) { return false; }
	
	if (_startTime == 0) { _startTime = _game->GetModeTm(); }
	_clearTime = _game->GetModeTm() - _startTime;

	return true;
}

bool Result::TimeRankRender() {
	std::string rankStr;
	if (_eTimeRank == ClearTimeRank::ksrank) rankStr = "S";
	else if (_eTimeRank == ClearTimeRank::karank) rankStr = "A";
	else if (_eTimeRank == ClearTimeRank::kbrank) rankStr = "B";
	else if (_eTimeRank == ClearTimeRank::kcrank) rankStr = "C";

	DrawFormatString(0, 100, GetColor(0, 255, 0),
		"time:%d \nrank:%s",
		_clearTime, rankStr.c_str());

	return true;
}

bool Result::EvaluateMaxDetection(float maxDetection) {
	if (maxDetection == 0) { return false; }
	else if (maxDetection <= static_cast<int>(MaxDetectionRank::kSrank)) { _eDetectionRank = MaxDetectionRank::kSrank; }
	else if (maxDetection <= static_cast<int>(MaxDetectionRank::kArank)) { _eDetectionRank = MaxDetectionRank::kArank; }
	else if (maxDetection <= static_cast<int>(MaxDetectionRank::kBrank)) { _eDetectionRank = MaxDetectionRank::kBrank; }
	else { _eDetectionRank = MaxDetectionRank::kCrank; }
	return true;
}

bool Result::DetectionRankProcess() {
	if(_game == nullptr) { return false; }
	auto server = _game->GetObjectServer();
	for (auto& detection : server->GetCommonSoldiers()) {
		if (_maxDetection < detection->GetDetectionLevel()) {
			_maxDetection = detection->GetDetectionLevel();
		}
	}
	return true;
}

bool Result::DetectionRankRender() {
	std::string rankStr;
	if (_eDetectionRank == MaxDetectionRank::kSrank) rankStr = "S";
	else if (_eDetectionRank == MaxDetectionRank::kArank) rankStr = "A";
	else if (_eDetectionRank == MaxDetectionRank::kBrank) rankStr = "B";
	else if (_eDetectionRank == MaxDetectionRank::kCrank) rankStr = "C";

	DrawFormatString(0, 140, GetColor(0, 255, 0),
		"ditection%f \nrank:%s",
		_maxDetection, rankStr.c_str());

	return true;
}

bool Result::ContinuousKillsProcess() {
	if (_game == nullptr) { return false; }

	auto server = _game->GetObjectServer();
	if (server == nullptr) { return false; }

	auto common = server->GetCommonSoldiers();
	if (common.empty()) { return false; }

	const int deathCooldownFrames = 60; // デスのクールダウン時間（フレーム数）
	const std::string deathState = "Death"; // デス状態の文字列

	for (auto& com : common) {
		auto ai = com->GetAIComponent();
		if (ai == nullptr) { continue; }

		auto state = ai->GetCurrentState()->GetName();
		if (_diedTime != 0 && _diedTime + deathCooldownFrames > _game->GetModeCount() && state == deathState) {
			// 連続キルの条件を満たさない場合はスキップ
			continue;
		}
		else if (state == deathState) {
			// 60フレーム以内にもう一度敵を倒した場合、連続キル数を増加
			if (_diedTime != 0 && _diedTime + deathCooldownFrames > _game->GetModeCount()) {
				_continuousKills++;
			}
			else {
				// 新しい連続キルのカウントを開始
				_continuousKills = 1;
			}
			_diedTime = _game->GetModeCount();
		}
		/*if (_diedTime != 0 && _diedTime + deathCooldownFrames > _game->GetModeCount() &&
			_bDeath == true) {
			_continuousKills++;
			_diedTime = _game->GetModeCount();
			_bDeath = false;
		}
		else if (_diedTime == 0 && _bDeath == false) {
			_continuousKills++;
			_diedTime = _game->GetModeCount();
			_bDeath = true;
		}
		else {
			_diedTime = 0;
			_bDeath = false;
		}*/
	}
	return true;
}

bool Result::ContinuousKillsRender() {

	int a = 0;
	if(_bDeath) a = 1;
	DrawFormatString(0, 180, GetColor(0, 255, 0),
		"continuousKills%d \ndeath%d",
		_continuousKills, a);

	return true;
}
