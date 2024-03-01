#include "UIClearTime.h"

#include "ApplicationMain.h"
#include "ApplicationGlobal.h"

UIClearTime::UIClearTime() {
	int cgNum[TIME_NUM];

	_mCg["colon"].push_back(res::LoadGraph("res/UI/Game/Timer/ui_timer_02.png"));
	res::LoadDivGraph("res/UI/Game/Timer/ui_timer_01.png", 10, 5, 2, 46, 70, cgNum);

	for(int i = 0; i < TIME_NUM; i++) {
		_mCg["number"].push_back(cgNum[i]);
	}
	_clearTime = gGlobal._result.GetClearTime();
	// clearTimeの数値から数字を取得する
	_mDigit["min1"] = _clearTime / 600000;
	_mDigit["min2"] = _clearTime / 60000;
	_mDigit["sec1"] = (_clearTime / 1000 - 60) / 10;
	_mDigit["sec2"] = _clearTime / 1000 % 10;
	_mDigit["mSec1"] = _clearTime / 100 % 10;
	_mDigit["mSec2"] = _clearTime / 10 % 10;
}

UIClearTime::~UIClearTime() {
	_mCg.clear();
	_mDigit.clear();
}

void UIClearTime::Render() {
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
	// タイマーの数字
	DrawExtendGraph(x, y, w, h, _mCg["number"][_mDigit["min1"]], TRUE);
	x += ConvertX(46);
	w = x + ConvertX(46);
	DrawExtendGraph(x, y, w, h, _mCg["number"][_mDigit["min2"]], TRUE);
	x += ConvertX(46);
	w = x + ConvertX(15);
	DrawExtendGraph(x, y, w, h, res::LoadGraph("res/UI/Game/Timer/ui_timer_02.png"), TRUE);
	x += ConvertX(15);
	w = x + ConvertX(46);
	DrawExtendGraph(x, y, w, h, _mCg["number"][_mDigit["sec1"]], TRUE);
	x += ConvertX(46);
	w = x + ConvertX(46);
	DrawExtendGraph(x, y, w, h, _mCg["number"][_mDigit["sec2"]], TRUE);
	x += ConvertX(46);
	w = x + ConvertX(15);
	DrawExtendGraph(x, y, w, h, res::LoadGraph("res/UI/Game/Timer/ui_timer_02.png"), TRUE);
	x += ConvertX(15);
	w = x + ConvertX(46);
	DrawExtendGraph(x, y, w, h, _mCg["number"][_mDigit["mSec1"]], TRUE);
	x += ConvertX(46);
	w = x + ConvertX(46);
	DrawExtendGraph(x, y, w, h, _mCg["number"][_mDigit["mSec2"]], TRUE);
}
