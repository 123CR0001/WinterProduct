#include"ModeColorIn.h"

ModeColorIn::ModeColorIn(int maxCnt,bool isBlack)
	:_frameMaxCnt(maxCnt)
	,_isBlack(isBlack)
{

}

bool ModeColorIn::Initialize() {
	_frameCnt = 0;
	_alpha = 0;
	_red = 255;
	_green = 255;
	_blue = 255;
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


	if (_frameCnt == -1) {

	}
	else if (_frameCnt > _frameMaxCnt) {
		ModeServer::GetInstance()->Del(this);
		_frameCnt = -1;
	}
	else {
		_alpha = EasingLinear(_frameCnt, 255, 0, _frameMaxCnt);
		_frameCnt++;
	}

	// このモードより下のレイヤーはProcess()を呼ばない
//	ModeServer::GetInstance()->SkipProcessUnderLayer();
	return true;
}

bool ModeColorIn::Render() {

	auto instance = ApplicationBase::GetInstance();

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, _alpha);	// 半透明モード
	DrawBox(0, 0, instance->DispSizeW(), instance->DispSizeH(), GetColor(_red, _green, _blue), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, _alpha);
	return true;
}