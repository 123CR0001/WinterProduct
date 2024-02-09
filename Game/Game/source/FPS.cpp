#include "FPS.h"

/*
	@brief	コンストラクタ
*/
FPS::FPS() {
	_startTime		= 0.0f;
	_count			= 0;
	_fps			= 0;
	_cntAverage		= 60;
	_configuredFPS	= 60;
}

/*
	@brief	デストラクタ
*/
FPS::~FPS() {
}

/*
	@brief	更新
	@return	true	：成功
*/
bool FPS::Update() {
	if (_count == 0) { //1フレーム目なら時刻を記憶
		_startTime = GetNowCount();
	}
	if (_count == _cntAverage) { //60フレーム目なら平均を計算する
		float t = GetNowCount();
		_fps = 1000.f / ((t - _startTime) / (float)_cntAverage);
		_count = 0;
		_startTime = t;
	}
	_count++;
	return true;
}

/*
	@brief	待機
*/
void FPS::WaitFPS() const {
	int tookTime = GetNowCount() - _startTime;	//かかった時間
	int waitTime = _count * 1000 / _configuredFPS - tookTime;	//待つべき時間
	if (waitTime > 0) {
		Sleep(waitTime);	//待機
	}
}

/*
	@brief	FPSの設定
	@param	fps	：FPS
*/
void FPS::SetFPS(float fps) {
	// 設定したFPSを保存
	_configuredFPS = _cntAverage = fps;
}
