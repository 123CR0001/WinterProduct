#include "FPS.h"

/*
	@brief	�R���X�g���N�^
*/
FPS::FPS() {
	_startTime		= 0.0f;
	_count			= 0;
	_fps			= 0;
	_cntAverage		= 60;
	_configuredFPS	= 60;
}

/*
	@brief	�f�X�g���N�^
*/
FPS::~FPS() {
}

/*
	@brief	�X�V
	@return	true	�F����
*/
bool FPS::Update() {
	if (_count == 0) { //1�t���[���ڂȂ玞�����L��
		_startTime = GetNowCount();
	}
	if (_count == _cntAverage) { //60�t���[���ڂȂ畽�ς��v�Z����
		float t = GetNowCount();
		_fps = 1000.f / ((t - _startTime) / (float)_cntAverage);
		_count = 0;
		_startTime = t;
	}
	_count++;
	return true;
}

/*
	@brief	�ҋ@
*/
void FPS::WaitFPS() const {
	int tookTime = GetNowCount() - _startTime;	//������������
	int waitTime = _count * 1000 / _configuredFPS - tookTime;	//�҂ׂ�����
	if (waitTime > 0) {
		Sleep(waitTime);	//�ҋ@
	}
}

/*
	@brief	FPS�̐ݒ�
	@param	fps	�FFPS
*/
void FPS::SetFPS(float fps) {
	// �ݒ肵��FPS��ۑ�
	_configuredFPS = _cntAverage = fps;
}
