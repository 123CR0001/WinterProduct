
#include "ApplicationMain.h"
#include "UIServer.h"

UIServer::UIServer() {

}

UIServer::~UIServer() {
	for(auto&& ui : _vUI) {
		delete ui;
	}
}

/*
	@brief	�R���X�g���N�^
	@return	true	�F����
	@return	false	�F���s
*/
bool UIServer::Initialize() {
	Clear();
	return true;
}

/*
	@brief	�f�X�g���N�^
	@return	true�F����
*/
bool UIServer::Terminate() {

	for (auto& ui : _vUI) {
		delete ui;
	}
	Clear();

	return true;
}

/*
	@brief	����
	@return	true�F����
*/
bool UIServer::Process() {

	_vUI.sort(uiSort);

	for (auto& ui : _vUI) {
		ui->Process();
	}

	return true;
}

/*
	@brief	�`��
	@return	true�F����
*/
bool UIServer::Render() {

	lstUIBase::iterator ite = _vUI.begin();
	for (auto& ui : _vUI) {
		if (!ui->_bView) { continue; }
		ui->Render();
	}

	return true;
}

/*
	@brief	UI�̒ǉ�
	@param	uiBase	�F�ǉ�����UI
	@param	param	�F�Q�Ƃ���N���X
	@param	cg		�F�摜�n���h��
	@param	x, y	�F���W
	@param	w, h	�F�摜�T�C�Y
	@param	layer	�F���C���[
	@param	name	�F�o�^��
*/
void UIServer::Add(UIBase* uiBase, void* param, int cg, float x, float y, float w, float h, int layer, std::string name) {
	_vUI.push_back(uiBase);
	uiBase->_param = param;
	uiBase->_cg = cg;
	uiBase->_x = x;
	uiBase->_y = y;
	uiBase->_w = w;
	uiBase->_h = h;
	uiBase->_layer = layer;
	uiBase->_uiName = name;
}

/*
	@brief	UI�̑S�폜
*/
void UIServer::Clear() {
	_vUI.clear();
}

/*
	@brief	UI�̌���
	@param	name�F�T������UI
*/
UIBase* UIServer::Search(std::string name) {
	lstUIBase::iterator ite;
	ite = _vUI.begin();
	for (; ite != _vUI.end(); ite++) {
		if ((*ite)->_uiName == name) { 
			return(*ite);
		}
	}
	return NULL;
}

/*
	@brief	�I�����̈ʒu���A�w�肵��UI�̈ʒu�Ɉړ�������
	@param	selectName	�F�ړ���Ɏw�肵����UI
	@param	get			�F�I����
*/
void UIServer::SelectPosAdjustment(std::string selectName, std::string get, int shiftValueX, int shiftValueY) {
	float posX = Search(selectName)->GetPosX();
	float posY = Search(selectName)->GetPosY();
	posX += shiftValueX;
	posY += shiftValueY;
	Search(get)->SetPos(posX, posY);
}


void UIServer::HideDisplay(UIBase* uiBase, bool isHide) {
	uiBase->_bView = isHide;
}


bool UIServer::IsRegistering(std::string searchName) {
	if (Search(searchName) == NULL) { return false; }
	return true;
}

void UIServer::Del(std::string delName) {
	UIBase* ui = Search(delName);
	lstUIBase::iterator ite = _vUI.begin();
	for (; ite != _vUI.end();) {
		if((*ite) == ui){ ite = _vUI.erase(ite); }
		else { ++ite; }
	}
}

/*
	@brief	X���W����ʃT�C�Y�ɍ��킹�ĕϊ�����
	@param	pos�Fx���W
*/
float UIServer::ConvertXPosToDispSize(float pos) {
	float disp = ApplicationMain::GetInstance()->DispSizeW();
	float result = disp * pos / 1920;
	return result;
}

/*
	@brief	Y���W����ʃT�C�Y�ɍ��킹�ĕϊ�����
	@param	pos�Fy���W
*/
float UIServer::ConvertYPosToDispSize(float pos) {
	float disp = ApplicationMain::GetInstance()->DispSizeH();
	float result = disp * pos / 1080;
	return result;
}