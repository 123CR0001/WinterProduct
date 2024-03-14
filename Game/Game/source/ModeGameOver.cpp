#include"ModeGameOver.h"
#include "ApplicationMain.h"
#include"ApplicationGlobal.h"
#include"ModeTitle.h"

#include "UIServer.h"
#include "UIDisplay.h"
#include "UIRetry.h"
#include "UIOverToTitle.h"

ModeGameOver::ModeGameOver(ModeGame* game) {
	// �Q�[�����[�h�̃|�C���^���󂯎��
	_game= game;
	// �Q�[�����[�h�̃X�e�[�W�ԍ����󂯎��
	_stage = _game->GetStage();
}

bool ModeGameOver::Initialize() {

	// �摜�̓ǂݍ���
	int cgContinue, cgExit, cgFrame;
	res::LoadDivGraph("res/UI/GameOver/ui_gameoverbg_01.png", 6, 1, 6, 1920, 1080, _cgBg);
	cgContinue	= res::LoadGraph("res/UI/GameOver/ui_continue_01.png");
	cgExit		= res::LoadGraph("res/UI/GameOver/ui_exit_01.png");
	cgFrame		= res::LoadGraph("res/UI/GameOver/ui_gameoverframe_01.png");

	// UIServer�̒ǉ�
	_uiServer = NEW UIServer();
	// UIServer�̏�����
	_uiServer->Initialize();

	/*	UI�̒ǉ�   UIBase				param    Hundle		 x		y	 w	  h	  layer				name */
	_uiServer->Add(NEW UIRetry(this),	nullptr, cgContinue, 448,	576, 448, 64, LOW_LAYER_VALUE,	"continue");
	_uiServer->Add(NEW UIOverToTitle(), nullptr, cgExit,	 1024,	576, 448, 64, LOW_LAYER_VALUE,	"exit");
	_uiServer->Add(NEW UIDisplay(),		nullptr, cgFrame,	 448,	576, 448, 64, BASIC_LAYER_VALUE,"get");

	// �I�����Ƃ��Ĉ������̂ɁA�ԍ�������U��
	_uiServer->Search("continue")->_selectNum = 0;
	_uiServer->Search("exit")->_selectNum = 1;

	// �J�[�\���̏����ʒu���w��
	_select = _uiServer->Search("continue")->_selectNum;

	// �A�j���[�V���������̕ϐ��̏�����
	_animCnt = 0;
	_frameCnt = 4;
	_motCnt = 0;

	//BGM���~�߂�
	gGlobal._sndServer.StopType(SoundItemBase::TYPE::BGM);
	return true;
}

bool ModeGameOver::Terminate() {
	delete _uiServer;
	return true;
}

bool ModeGameOver::Process() {
	base::Process();

	// UI�̏���
	_uiServer->Process();

	auto trg = ApplicationMain::GetInstance()->GetPad()->GetTrgButton();

	// �㉺�ŃJ�[�\�����ړ�����
	if(trg & INPUT__DPAD_LEFT) { _select--; }
	if(trg & INPUT__DPAD_RIGHT) { _select++; }

	// �I���ʒu���㉺���[�v
	_select = (_select + MAX_OPTIONS) % MAX_OPTIONS;

	// �I������
	for(auto& ui : _uiServer->_vUI) {
		if(ui->_selectNum == -1) { continue; }					// ���ڂɎw�肳��Ă��Ȃ����̂̓X�L�b�v(-1�̂��͎̂w�肳��Ă��Ȃ�)
		if(ui->_selectNum == _select) {							// ui���Ɋ���U��ꂽ�ԍ���_select�̐������������ۂ�
			std::string name = ui->_uiName;						// �I�����Ă��鍀�ڂ̖��O
			_uiServer->SelectPosAdjustment(name, "get", 0, 0);	// �I�𒆂̍��ڂɃJ�[�\�����d�˂�
			if(trg & INPUT_A) { ui->Selected(); }				// A�{�^���őI�����ڂ̏������s�� 
			break;
		}
	}

	// �w�i�̃A�j���[�V��������
	_animCnt++;
	if(_frameCnt <= _animCnt) {
		if(_motCnt == 5) { _motCnt = 0; }
		_motCnt++;
		_animCnt = 0;
	}

	return true;
}

bool ModeGameOver::Render() {
	base::Render();

	// ��ʃT�C�Y�ɍ��킹�ĉ摜���g��k��
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

	// �w�i�̕`��
	float w = ConvertX(1920);
	float h = ConvertY(1080);
	DrawExtendGraph(0, 0, w, h, _cgBg[_motCnt], TRUE);

	// UI�̕`��
	_uiServer->Render();

	return true;
}