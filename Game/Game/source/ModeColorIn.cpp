#include"ModeColorIn.h"

ModeColorIn::ModeColorIn(int maxCnt,bool isBlack)
	:_frameMaxCnt(maxCnt)
	,_isBlack(isBlack)
{

}

bool ModeColorIn::Initialize() {
	_frameCnt = 0;
	_alpha = 0;

	// ���̏ꍇ��RGB��255�ɂ���
	_red = 255;
	_green = 255;
	_blue = 255;

	// ���̏ꍇ��RGB��0�ɂ���
	if (_isBlack) {
		_red = 0;
		_green = 0;
		_blue = 0;
	}
	return true;
}

bool ModeColorIn::Terminate() {

	return true;
}

bool ModeColorIn::Process() {

	// �t���[���J�E���g���ő�l�ɒB������A���g���폜����
	if (_frameCnt >= _frameMaxCnt) {
		ModeServer::GetInstance()->Del(this);
	}
	else {
		//�A���t�@�l���v�Z
		_alpha = static_cast<int>(EasingLinear((float)_frameCnt, 255, 0, (float)_frameMaxCnt));

		//�t���[���J�E���g��i�߂�
		_frameCnt++;
	}

	return true;
}

bool ModeColorIn::Render() {

	auto instance = ApplicationBase::GetInstance();

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, _alpha);	// ���������[�h
	DrawBox(0, 0, instance->DispSizeW(), instance->DispSizeH(), GetColor(_red, _green, _blue), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, _alpha);
	return true;
}