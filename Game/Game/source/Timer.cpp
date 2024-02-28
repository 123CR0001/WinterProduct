#include "Timer.h"

Timer::Timer() {
	_setTimeS = 0;
	_setTimeMS = 0;
	_second = 0;
	_mSecond = 0;
	_stopTime = 0;
	_bStart = false;
	_dxStartCnt = 0;
}

Timer::~Timer() {
}

bool Timer::Process() {
	// タイマーがスタートしていない場合はfalseを返す
	if (!_bStart) { return false; }

	// 現在の時間を取得
	int nowTimeS = (GetNowCount() - _dxStartCnt) / 1000;
	int nowTimeMS = GetNowCount() - _dxStartCnt;


	// タイマーがセットされている場合は時間を減らす
	if (_second > 0) _second = _setTimeS - nowTimeS;
	if (_mSecond > 0) _mSecond = _setTimeMS - nowTimeMS;

	// タイマーが0になった場合はタイマーを止める
	if (_second <= 0 && _mSecond <= 0) {
		_bStart = false;
		Reset();
		return true;
	}

	return true;
}

bool Timer::Start() {
	// タイマーがスタートしている場合はfalseを返す
	 if (_bStart || _second <= 0 && _mSecond <= 0) return false;

	_bStart = true;
	_dxStartCnt = GetNowCount();

	return true;
}

bool Timer::Stop() {
	// タイマーがスタートしていない場合はfalseを返す
	if (!_bStart) { return false; }

	// タイマーがスタートしている場合はfalse
	_bStart = false;
	// タイマーを止めた時間を記録
	_stopTime = GetNowCount() ;

	return true;
}

bool Timer::Reset() {
	// タイマーがスタートしている場合はfalseを返す
	if (_bStart) { return false; }

	// タイマーをリセットする
	_setTimeS = 0;
	_setTimeMS = 0;
	_second = 0;
	_mSecond = 0;
	_stopTime = 0;

	return true;
}

std::string Timer::CheckTimeUnit() {
	// タイマーがセットされていない場合は"not run"を返す
	std::string result = "not run";
	// タイマーがセットされている場合は時間の単位を返す
	if (_setTimeS > 0) { result = "second"; }
	else if(_setTimeMS > 0) { result = "mSecond"; }

	return result;
}


bool Timer::AddS(int second) {
	// タイマーがセットされていない場合はfalseを返す
	if (_setTimeS <= 0) { return false; }
	// タイマーに時間を追加する
	_setTimeS += second;
	return true;
}

bool Timer::AddMS(int mSecond) {
	// タイマーがセットされていない場合はfalseを返す
	if (_setTimeMS <= 0) { return false; }
	// タイマーに時間を追加する
	_setTimeMS += mSecond;
	return true;
}
