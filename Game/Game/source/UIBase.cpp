
#include "ApplicationMain.h"
#include "UIBase.h"

/*
	@brief	�R���X�g���N�^
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
	@brief	�f�X�g���N�^
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
	@brief	���ڂ����肵����Ă΂��
	@return	0�F�p���@1�F����
*/
int UIBase::Selected() {
	return 0;
}

/*
	@brief	������x�X���C�h������
	@param	initPosX�F�J�n�ʒu�iX���W�j�@
	@param	initPosY�F�J�n�ʒu�iY���W�j
	@param	endPosX�F�I���ʒu�iX���W�j�@
	@param	endPosY�F�I���ʒu�iY���W�j
	@param	frame�F�ړ��ɂ�����t���[����
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

