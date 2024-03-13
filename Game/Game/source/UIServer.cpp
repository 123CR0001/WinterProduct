
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
	@brief	コンストラクタ
	@return	true	：成功
	@return	false	：失敗
*/
bool UIServer::Initialize() {
	Clear();
	return true;
}

/*
	@brief	デストラクタ
	@return	true：成功
*/
bool UIServer::Terminate() {

	for (auto& ui : _vUI) {
		delete ui;
	}
	Clear();

	return true;
}

/*
	@brief	処理
	@return	true：成功
*/
bool UIServer::Process() {

	_vUI.sort(uiSort);

	for (auto& ui : _vUI) {
		ui->Process();
	}

	return true;
}

/*
	@brief	描画
	@return	true：成功
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
	@brief	UIの追加
	@param	uiBase	：追加するUI
	@param	param	：参照するクラス
	@param	cg		：画像ハンドル
	@param	x, y	：座標
	@param	w, h	：画像サイズ
	@param	layer	：レイヤー
	@param	name	：登録名
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
	@brief	UIの全削除
*/
void UIServer::Clear() {
	_vUI.clear();
}

/*
	@brief	UIの検索
	@param	name：探したいUI
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
	@brief	選択肢の位置を、指定したUIの位置に移動させる
	@param	selectName	：移動先に指定したいUI
	@param	get			：選択肢
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
	@brief	X座標を画面サイズに合わせて変換する
	@param	pos：x座標
*/
float UIServer::ConvertXPosToDispSize(float pos) {
	float disp = ApplicationMain::GetInstance()->DispSizeW();
	float result = disp * pos / 1920;
	return result;
}

/*
	@brief	Y座標を画面サイズに合わせて変換する
	@param	pos：y座標
*/
float UIServer::ConvertYPosToDispSize(float pos) {
	float disp = ApplicationMain::GetInstance()->DispSizeH();
	float result = disp * pos / 1080;
	return result;
}