#include"ModeColorOut.h"
#include"ModeColorIn.h"

ModeColorOut::ModeColorOut(ModeColorIn* modeColorIn, std::function<void()>func,int maxCnt)
	:_modeColorIn(modeColorIn)
	,_frameMaxCnt(maxCnt)
	,_func(func)
{

}

bool ModeColorOut::Initialize() {
	_frameCnt = 0;
	_alpha = 0;
	_red = 0;
	_green = 0;
	_blue = 0;
	return true;
}

bool ModeColorOut::Terminate() {
	
	return true;
}

bool ModeColorOut::Process() {

	if (_frameCnt == -1) {

	}
	else if(_frameCnt == _frameMaxCnt) {
		ModeServer::GetInstance()->Add(_modeColorIn, 1000, "ColorIn");
		_frameCnt++;
	}
	else if (_frameCnt > _frameMaxCnt) {
		ModeServer::GetInstance()->Del(this);

		
		if (_func) { _func(); }

		_frameCnt = -1;
	}
	else {
		_alpha = EasingLinear((float)_frameCnt, 0, 255, (float)_frameMaxCnt);
		_frameCnt++;
	}

	// このモードより下のレイヤーはProcess()を呼ばない
//	ModeServer::GetInstance()->SkipProcessUnderLayer();
	return true;
}

bool ModeColorOut::Render() {

	auto instance = ApplicationBase::GetInstance();

	SetDrawBlendMode(DX_BLENDMODE_ALPHA,_alpha);	// 半透明モード
	DrawBox(0, 0, instance->DispSizeW(), instance->DispSizeH(), GetColor(_red, _green, _blue), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, _alpha);
	return true;
}