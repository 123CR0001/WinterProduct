#include"TimeLine.h"

TimeLine::TimeLine()
	:_frameCnt(0)
{}

TimeLine::~TimeLine(){
	_funcs.clear();
}

void TimeLine::Process() {

	//�o�^���ꂽ�֐����Ȃ��ꍇ�͉������Ȃ�
	if (_funcs.empty()) { return; }

	auto iter = _funcs.find(_frameCnt);

	if (iter != _funcs.end()) {
		_funcs[_frameCnt]();

		//����������A�폜
		_funcs.erase(iter);
	}

	//�t���[���J�E���g��i�߂�
	if (_frameCnt < INT_MAX) {
		_frameCnt++;
	}
	//�t���[���J�E���g���ő�l�ɒB������A0�ɖ߂�
	else {
		_frameCnt = 0;
	}
}