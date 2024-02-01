
#include "ApplicationMain.h"
#include "UIBlink.h"

UIBlink::UIBlink() {
	_alphaVal = ALPHA_VAL_LOWLIMIT;
	_blinkFrame = 60;
	_blinkCnt = 0;
	_state = STATE::kUp;
}

void UIBlink::Process() {

	if (_state == STATE::kUp) {
		_alphaVal = EasingLinear(_blinkCnt, ALPHA_VAL_LOWLIMIT, ALPHA_VAL_HIGHLIMIT, _blinkFrame);
		if (_alphaVal == ALPHA_VAL_HIGHLIMIT) {
			_state = STATE::kDown; 
			_blinkCnt = 0;
		}
	}
	else if (_state == STATE::kDown) {
		_alphaVal = EasingLinear(_blinkCnt, ALPHA_VAL_HIGHLIMIT, ALPHA_VAL_LOWLIMIT, _blinkFrame);
		if (_alphaVal == ALPHA_VAL_LOWLIMIT) {
			_state = STATE::kUp; 
			_blinkCnt = 0;
		}
	}
	
	if (_blinkCnt < _blinkFrame) {
		_blinkCnt++;
	}
	else { _blinkCnt = 0; }


}

void UIBlink::Render() {
	auto ConvertX = [](float posX) {
		float disp = ApplicationMain::GetInstance()->DispSizeW();
		float result = disp * posX / 1920;
		return result;
	};
	auto ConvertY = [](float posY) {
		float disp = ApplicationMain::GetInstance()->DispSizeH();
		float result = disp * posY / 1080;
		return result;
	};

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, _alphaVal);
	float x = ConvertX(_x);
	float y = ConvertY(_y);
	float w = x + ConvertX(_w);
	float h = y + ConvertY(_h);
	DrawExtendGraph(x, y, w, h, _cg, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, _alphaVal);
}

