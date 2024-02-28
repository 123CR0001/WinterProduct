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
	// �^�C�}�[���X�^�[�g���Ă��Ȃ��ꍇ��false��Ԃ�
	if (!_bStart) { return false; }

	// ���݂̎��Ԃ��擾
	int nowTimeS = (GetNowCount() - _dxStartCnt) / 1000;
	int nowTimeMS = GetNowCount() - _dxStartCnt;


	// �^�C�}�[���Z�b�g����Ă���ꍇ�͎��Ԃ����炷
	if (_second > 0) _second = _setTimeS - nowTimeS;
	if (_mSecond > 0) _mSecond = _setTimeMS - nowTimeMS;

	// �^�C�}�[��0�ɂȂ����ꍇ�̓^�C�}�[���~�߂�
	if (_second <= 0 && _mSecond <= 0) {
		_bStart = false;
		Reset();
		return true;
	}

	return true;
}

bool Timer::Start() {
	// �^�C�}�[���X�^�[�g���Ă���ꍇ��false��Ԃ�
	 if (_bStart || _second <= 0 && _mSecond <= 0) return false;

	_bStart = true;
	_dxStartCnt = GetNowCount();

	return true;
}

bool Timer::Stop() {
	// �^�C�}�[���X�^�[�g���Ă��Ȃ��ꍇ��false��Ԃ�
	if (!_bStart) { return false; }

	// �^�C�}�[���X�^�[�g���Ă���ꍇ��false
	_bStart = false;
	// �^�C�}�[���~�߂����Ԃ��L�^
	_stopTime = GetNowCount() ;

	return true;
}

bool Timer::Reset() {
	// �^�C�}�[���X�^�[�g���Ă���ꍇ��false��Ԃ�
	if (_bStart) { return false; }

	// �^�C�}�[�����Z�b�g����
	_setTimeS = 0;
	_setTimeMS = 0;
	_second = 0;
	_mSecond = 0;
	_stopTime = 0;

	return true;
}

std::string Timer::CheckTimeUnit() {
	// �^�C�}�[���Z�b�g����Ă��Ȃ��ꍇ��"not run"��Ԃ�
	std::string result = "not run";
	// �^�C�}�[���Z�b�g����Ă���ꍇ�͎��Ԃ̒P�ʂ�Ԃ�
	if (_setTimeS > 0) { result = "second"; }
	else if(_setTimeMS > 0) { result = "mSecond"; }

	return result;
}


bool Timer::AddS(int second) {
	// �^�C�}�[���Z�b�g����Ă��Ȃ��ꍇ��false��Ԃ�
	if (_setTimeS <= 0) { return false; }
	// �^�C�}�[�Ɏ��Ԃ�ǉ�����
	_setTimeS += second;
	return true;
}

bool Timer::AddMS(int mSecond) {
	// �^�C�}�[���Z�b�g����Ă��Ȃ��ꍇ��false��Ԃ�
	if (_setTimeMS <= 0) { return false; }
	// �^�C�}�[�Ɏ��Ԃ�ǉ�����
	_setTimeMS += mSecond;
	return true;
}
