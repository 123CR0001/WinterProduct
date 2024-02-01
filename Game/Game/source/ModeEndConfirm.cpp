
#include "ApplicationMain.h"
#include "ModeEndConfirm.h"
#include "UIServer.h"
#include "UIGameEnd.h"
#include "UINo.h"
#include "UIDisplay.h"
#include "UIBlink.h"

/*
	@brief	�R���X�g���N�^
	@return	true�F����
*/
bool ModeEndConfirm::Initialize() {
	_ui = new UIServer();

	void* mdTitle = ModeServer::GetInstance()->Get("title");

	// UI�p�̉摜�n���h��
	int cgYes, cgNo, cgGet, cgBg, cgQuit;
	cgYes	= res::LoadGraph("res/UI/Confirm/ui_quityes_01.png");
	cgNo	= res::LoadGraph("res/UI/Confirm/ui_quitno_01.png");
	cgGet	= res::LoadGraph("res/UI/Confirm/ui_target_02.png");
	cgBg	= res::LoadGraph("res/UI/Confirm/confirm_disply.png");
	cgQuit	= res::LoadGraph("res/UI/Confirm/ui_quitscreen_01.png");

	/*           UIBase		  param    Hundle	x     y    w     h	   layer				name */
	_ui->Add(new UIGameEnd(), mdTitle, cgYes,	640,  768, 255,	 63,   HIGH_LAYER_VALUE,	"yes");
	_ui->Add(new UINo(),	  nullptr, cgNo,	1024, 768, 255,	 63,   HIGH_LAYER_VALUE,	"no");
	_ui->Add(new UIBlink(), nullptr, cgGet,	640,  768, 255,	 63,   BASIC_LAYER_VALUE,	"get_confirm");
	_ui->Add(new UIDisplay(), nullptr, cgBg,	0,	  0,   1920, 1080, LOW_LAYER_VALUE ,	"bg_confirm");
	_ui->Add(new UIDisplay(), nullptr, cgQuit,	0,	  256, 1920, 640,  LOW_LAYER_VALUE,		"quit_confirm");

	_ui->Search("yes")->_selectNum = 0;
	_ui->Search("no")->_selectNum = 1;

	_select = _ui->Search("no")->_selectNum;

	return true;
}

/*
	@brief	�f�X�g���N�^
	@return	true�F����
*/
bool ModeEndConfirm::Terminate() {
	base::Terminate();
	delete _ui;
	return true;
}

/*
	@brief	�m�F����
	@return	true�F����
*/
bool ModeEndConfirm::Process() {
	base::Process();

	ModeServer::GetInstance()->SkipProcessUnderLayer();

	_ui->Process();

	int trg = ApplicationMain::GetInstance()->GetPad()->GetTrgButton();

	// �㉺�ŃJ�[�\�����ړ�����
	if (trg & INPUT__DPAD_LEFT) { _select--; }
	if (trg & INPUT__DPAD_RIGHT) { _select++; }

	// �J�[�\���ʒu���㉺���[�v
	_select = (_select + MAX_OPTIONS) % MAX_OPTIONS;

	// �I�����̈ʒu��ύX����
	int close = 0;
	// �I������
	for (auto& ui : _ui->_vUI) {
		if (ui == _ui->Search("get_confirm") || ui->_layer < BASIC_LAYER_VALUE) { continue; }
			if (ui->_selectNum == _select) {
			std::string name = ui->_uiName;	// �I�����Ă��鍀�ڂ̖��O
				_ui->SelectPosAdjustment(name, "get_confirm", 0, 0);
			if (trg & INPUT_A) {	// ����őI�����ڂ�Selected()���Ă�
				close = _ui->Search(name)->Selected();
			}
			break;
		}
	}

	if (close != 0) {
		ModeServer::GetInstance()->Del(this);
	}

	return true;
}

/*
	@brief	�`�揈��
	@return	true�F����
*/
bool ModeEndConfirm::Render() {
	base::Render();

	_ui->Render();

	return true;
}
