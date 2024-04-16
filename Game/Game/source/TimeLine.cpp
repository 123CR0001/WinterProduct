#include"TimeLine.h"

TimeLine::TimeLine()
	:_frameCnt(0)
{}

TimeLine::~TimeLine(){
	_funcs.clear();
}

void TimeLine::Process() {

	//登録された関数がない場合は何もしない
	if (_funcs.empty()) { return; }

	auto iter = _funcs.find(_frameCnt);

	if (iter != _funcs.end()) {
		_funcs[_frameCnt]();

		//処理したら、削除
		_funcs.erase(iter);
	}

	//フレームカウントを進める
	if (_frameCnt < INT_MAX) {
		_frameCnt++;
	}
	//フレームカウントが最大値に達したら、0に戻す
	else {
		_frameCnt = 0;
	}
}