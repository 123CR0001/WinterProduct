
#include "ApplicationMain.h"
#include "UILightsTimer.h"
#include "Timer.h"

UILightsTimer::UILightsTimer() {
	_mCg["colon"].push_back( res::LoadGraph("res/UI/Game/Timer/ui_timer_02.png"));
	 res::LoadDivGraph("res/UI/Game/Timer/ui_timerbg_01.png",5,1,5,600,150, &_mCg["frame"][0]);
	 res::LoadDivGraph("res/UI/Game/Timer/ui_timer_01.png", 10, 5, 2, 46, 70, &_mCg["number"][0]);
	_timer = new Timer();
	_bFrameOpen = false;
	_animCnt = 0;
	_frameCnt = 6;
	_motCnt = 0;
}

UILightsTimer::~UILightsTimer() {
	// メモリ解放
	delete _timer;
	_mCg.clear();
	_mDigit.clear();
}

void UILightsTimer::Process() {
	// タイマーの処理
	_timer->Process();
	
	if (_bFrameOpen) { AnimProcess(); }

	if (OpenFrame() && _timer->CheckTimeUnit() == "mSecond") {
		int nowTime = _timer->GetTimeMS();
		_mDigit["sec1"] = nowTime / 10000;
		_mDigit["sec2"] = nowTime / 1000 % 10;
		_mDigit["mSec1"] = nowTime / 100 % 10;
		_mDigit["mSec2"] = nowTime /10 % 10;
	}

}

void UILightsTimer::Render() {

	auto ConvertX = [](float posX) {
		float disp = ApplicationMain::GetInstance()->DispSizeW();
		float result = disp * posX / 1920;
		return result;
		};
	auto ConvertY = [](float posY) {
		float disp = ApplicationMain::GetInstance()->DispSizeH();
		float result = disp * posY / 1080;
		return result;
		};
	int x = ConvertX(_x);
	int y = ConvertY(_y);
	int w = ConvertX(_x) + ConvertX(_w);
	int h = ConvertY(_y) + ConvertY(_h);
	// タイマーの背景
	DrawExtendGraph(x, y, w, h, _mCg["frame"][_motCnt], TRUE);

	if (OpenFrame()) {
		x += ConvertX(120 + 5);
		y += ConvertY(5);
		w = x + ConvertX(46);
		h = y + ConvertY(140);
		// タイマーの数字
		DrawExtendGraph(x, y, w, h, _mCg["number"][_mDigit["sec1"]], TRUE);
		x += ConvertX(46);
		w = x + ConvertX(46);
		DrawExtendGraph(x, y, w, h, _mCg["number"][_mDigit["sec2"]], TRUE);
		x += ConvertX(46);
		w = x + ConvertX(15);
		DrawExtendGraph(x, y, w, h, _mCg["colon"][0], TRUE);	
		x += ConvertX(15);
		w = x + ConvertX(46);
		DrawExtendGraph(x, y, w, h, _mCg["number"][_mDigit["mSec1"]], TRUE);
		x += ConvertX(46);
		w = x + ConvertX(46);
		DrawExtendGraph(x, y, w, h, _mCg["number"][_mDigit["mSec2"]], TRUE);
	}
}

int UILightsTimer::Selected() {
	// フレームのオープン、クローズ
	_bFrameOpen = !_bFrameOpen;
	// タイマーの設定
	_timer->SetTimeMS(10000);

	return 0;
}

bool UILightsTimer::OpenFrame() {
	if (_motCnt == _mCg["frame"].size()) {
		_bFrameOpen = false;
		return true;
	}
	return false;
}

void UILightsTimer::AnimProcess() {
	_animCnt++;
	if (_animCnt > _frameCnt) {
		_animCnt = 0;
		_motCnt++;
	}
	
	if (OpenFrame()) { _timer->Start(); }
}
