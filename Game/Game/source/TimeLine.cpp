#include"TimeLine.h"

TimeLine::TimeLine()
	:_frameCnt(0)
{}

TimeLine::~TimeLine(){
	_funcs.clear();
}

bool TimeLine::Process() {

	if (_funcs.empty()) { return true; }

	auto iter = _funcs.find(_frameCnt);

	if (iter != _funcs.end()) {
		_funcs[_frameCnt]();

		//����������A�폜
		_funcs.erase(iter);
	}

	if (_frameCnt < INT_MAX) {
		_frameCnt++;
	}
	else {
		return false;
	}

	return true;

}