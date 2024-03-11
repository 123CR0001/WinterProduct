#include"TimeLine.h"

TimeLine::TimeLine()
	:_frameCnt(0)
{}

TimeLine::~TimeLine(){}

bool TimeLine::Process() {

	if (_funcs.empty()) { return true; }

	if (_funcs.find(_frameCnt) != _funcs.end()) {
		_funcs[_frameCnt]();
	}

	if (_frameCnt < INT_MAX) {
		_frameCnt++;
	}
	else {
		return false;
	}

	return true;

}