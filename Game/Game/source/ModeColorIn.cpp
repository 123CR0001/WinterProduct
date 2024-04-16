#include"ModeColorIn.h"

ModeColorIn::ModeColorIn(int maxCnt,bool isBlack)
	:_frameMaxCnt(maxCnt)
	,_isBlack(isBlack)
{

}

bool ModeColorIn::Initialize() {
	_frameCnt = 0;
	_alpha = 0;

	// 白の場合はRGBを255にする
	_red = 255;
	_green = 255;
	_blue = 255;

	// 黒の場合はRGBを0にする
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

	// フレームカウントが最大値に達したら、自身を削除する
	if (_frameCnt >= _frameMaxCnt) {
		ModeServer::GetInstance()->Del(this);
	}
	else {
		//アルファ値を計算
		_alpha = static_cast<int>(EasingLinear((float)_frameCnt, 255, 0, (float)_frameMaxCnt));

		//フレームカウントを進める
		_frameCnt++;
	}

	return true;
}

bool ModeColorIn::Render() {

	auto instance = ApplicationBase::GetInstance();

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, _alpha);	// 半透明モード
	DrawBox(0, 0, instance->DispSizeW(), instance->DispSizeH(), GetColor(_red, _green, _blue), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, _alpha);
	return true;
}