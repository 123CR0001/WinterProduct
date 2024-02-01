
#include "ApplicationMain.h"
#include "UIBase.h"

/*
	@brief	コンストラクタ
*/
UIBase::UIBase() {
	_param = NULL;
	_cg = 0;
	_x = 0;
	_y = 0;
	_w = 0;
	_h = 0;
	_layer = 0;
	_uiName = "";
	_selectNum = -1;
	_bView = true;
}

/*
	@brief	デストラクタ
*/
void UIBase::Terminate() {

}


void UIBase::Process() {

}

void UIBase::Render() {
	auto x = [](float posX) {
		float disp = ApplicationMain::GetInstance()->DispSizeW();
		float result = disp * posX / 1920;
		return result;
	};
	auto y = [](float posY) {
		float disp = ApplicationMain::GetInstance()->DispSizeH();
		float result = disp * posY / 1080;
		return result;
	};
	DrawExtendGraph(x(_x), y(_y), x(_x) + x(_w), y(_y) + y(_h), _cg, TRUE);
}

/*
	@brief	項目を決定したら呼ばれる
	@return	0：継続　1：決定
*/
int UIBase::Selected() {
	return 0;
}

/*
	@brief	もう一度スライドさせる
	@param	initPosX：開始位置（X座標）　
	@param	initPosY：開始位置（Y座標）
	@param	endPosX：終着位置（X座標）　
	@param	endPosY：終着位置（Y座標）
	@param	frame：移動にかかるフレーム数
*/
void UIBase::SetSlideUIPosition(float initPosX, float initPosY, float endPosX, float endPosY, int frame) {
	_initPosX = initPosX;
	_initPosY = initPosY;
	_endPosX = endPosX;
	_endPosY = endPosY;
	_cnt = 0;
	_frame = frame;
}

bool UIBase::IsSlide() {
	if (_cnt < _frame) { return true; }
	return false;
}

