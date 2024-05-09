#include"ModeClear.h"
#include"ModeTitle.h"

#include "ApplicationMain.h"
#include"ApplicationGlobal.h"

#include "UIServer.h"
#include "UIDisplay.h"
#include "UIBlink.h"

#include"ModeColorIn.h"
#include"ModeColorOut.h"
#include"ModeStageSelect.h"
#include"ModeGame.h"

#include"ButtonServer.h"
#include"Button.h"

#include"SpriteText.h"
#include"SpriteNumber.h"
#include"TransformAnimation.h"
#include"OpacityAnimation.h"
#include"EasingNumber.h"

#include"UIScreen.h"
#include"TimeLine.h"
#include"UIHrsMinSec.h"
#include"UISpriteText.h"

#include"ClearData.h"

ModeClear::ModeClear(std::shared_ptr<ClearData> data)
	:_clearData(data)
	,_uiScreen(NEW UIScreen())
	,_buttonServer(NEW ButtonServer())
	,_timeLine(NEW TimeLine())
{
	//UI�̃Z�b�g
	SetUI();

	//�{�^���̃Z�b�g
	SetButton();

}

bool ModeClear::Initialize() {
	//BGM�̍Đ�
	gGlobal._sndServer.Play("BGM_08");
	return true;
}

bool ModeClear::Terminate() {
	delete _buttonServer;
	delete _uiScreen;
	delete _timeLine;
	return true;
}

bool ModeClear::Process() {
	base::Process();

	//�X�V
	_uiScreen->Process();
	_buttonServer->Process();
	_timeLine->Process();

	return true;
}

bool ModeClear::Render() {
	base::Render();

	_uiScreen->Draw();
	_buttonServer->Draw();

	return true;
}

void ModeClear::SetUI() {


	//�f�[�^�O�g
	{
		auto text = NEW SpriteText(
			ResourceServer::LoadGraph("res/UI/Result/ui_scorebg_01.png"),
			Transform2(Vector2(3000.f * SCREEN_WIDTH_MAG, 244.f * SCREEN_HEIGHT_MAG)),
			Vector2(896.f * SCREEN_WIDTH_MAG, 366.f * SCREEN_HEIGHT_MAG)
		);
		text->AddAnimation(NEW TransformAnimation(text, 60, Transform2(Vector2(1344.f * SCREEN_WIDTH_MAG, 244.f * SCREEN_HEIGHT_MAG))));
		_uiScreen->AddUI(NEW UISpriteText(text));
	}

	//�{�^���O�g
	{
		auto text = NEW SpriteText(
			ResourceServer::LoadGraph("res/UI/Result/ui_scorebg_02.png"),
			Transform2(Vector2(3000.f * SCREEN_WIDTH_MAG, 896.f * SCREEN_HEIGHT_MAG)),
			Vector2(896.f * SCREEN_WIDTH_MAG, 232.f * SCREEN_HEIGHT_MAG)
		);
		text->AddAnimation(NEW TransformAnimation(text, 60, Transform2(Vector2(1344.f * SCREEN_WIDTH_MAG, 896.f * SCREEN_HEIGHT_MAG))));
		_uiScreen->AddUI(NEW UISpriteText(text));
	}

	//�N���A�^�C��
	{
		auto func = [=]()mutable {
			_uiScreen->AddUI(
				NEW UIHrsMinSec(
					30,
					_clearData->clearSecondTime,
					Transform2(Vector2(1766.f * SCREEN_WIDTH_MAG,192.5f * SCREEN_HEIGHT_MAG)),
					Vector2(46.f * SCREEN_WIDTH_MAG,70.f * SCREEN_HEIGHT_MAG)
				)
			);
		};
		//���̊֐����Ăяo����Ă���AMyUIServer��Process()��70��Ă΂ꂽ��A��������
		_timeLine->AddLine(70, func);
	}

	//�ō����m�x
	{
		auto func = [=]() mutable{
			SpriteNumber* number = NEW SpriteNumber(static_cast<int>(_clearData->maxDetectionLevel * 100.f));
			number->AddAnimation(NEW EasingNumber(number, 30)); 
			number->LoadDivNumber("res/UI/Result/ui_timer_01.png",5,2,46,70);
			number->SetSize(Vector2(46.f * SCREEN_WIDTH_MAG, 70.f * SCREEN_HEIGHT_MAG));
			number->SetPos(Vector2(1709.f * SCREEN_WIDTH_MAG, 282.5f * SCREEN_HEIGHT_MAG));
			_uiScreen->AddUI(NEW UISpriteText(number));
		};

		//���̊֐����Ăяo����Ă���AMyUIServer��Process()��120��Ă΂ꂽ��A��������
		_timeLine->AddLine(120, func);
	}

	//�ō��L���R���{
	{
		auto func = [=]() mutable {
			SpriteNumber* number = NEW SpriteNumber(_clearData->maxCombo);
			number->AddAnimation(NEW EasingNumber(number, 30));
			number->LoadDivNumber("res/UI/Result/ui_timer_01.png", 5, 2, 46, 70);
			number->SetSize(Vector2(46.f * SCREEN_WIDTH_MAG, 70.f * SCREEN_HEIGHT_MAG));
			number->SetPos(Vector2(1709.f * SCREEN_WIDTH_MAG, 372.5f * SCREEN_HEIGHT_MAG));
			_uiScreen->AddUI(NEW UISpriteText(number));
		};

		//���̊֐����Ăяo����Ă���AMyUIServer��Process()��180��Ă΂ꂽ��A��������
		_timeLine->AddLine(180, func);
	}

	//�G���u����
	{
		auto func = [=]() mutable {
			std::string fileName = "res/UI/Result/Emblem/ui_rankemblem_";
			fileName += _clearData->GetRank();
			fileName += ".png";
			SpriteText* text = NEW SpriteText(
				ResourceServer::LoadGraph(fileName.c_str()),
					//			��]	�g��	���W
					Transform2(0.f,		3.f,	Vector2(1344.f * SCREEN_WIDTH_MAG, 589.5f * SCREEN_HEIGHT_MAG)),
					Vector2(640.f * SCREEN_WIDTH_MAG, 230.f * SCREEN_HEIGHT_MAG),
					0.f
			);
			text->AddAnimation(NEW TransformAnimation(text,30,Transform2(0.f, 1.f, Vector2(SCREEN_WIDTH_MAG * 1344.f, SCREEN_HEIGHT_MAG * 589.5f))));
			text->AddAnimation(NEW OpacityAnimation(text, 30, 1.f));
			_uiScreen->AddUI(NEW UISpriteText(text));
		};

		//���̊֐����Ăяo����Ă���AMyUIServer��Process()��240��Ă΂ꂽ��A��������
		_timeLine->AddLine(240, func);
	}

	//�w�i
	{
		SpriteText* text = NEW SpriteText();
		text->SetHandle(ResourceServer::LoadGraph("res/UI/Result/ui_resultbg_01.png"));
		text->SetSize(Vector2(static_cast<float>(SCREEN_WIDTH),static_cast<float>(SCREEN_HEIGHT)));
		text->SetPos(Vector2(static_cast<float>(SCREEN_WIDTH / 2), static_cast<float>(SCREEN_HEIGHT / 2)));

		_uiScreen->AddUI(NEW UISpriteText(text, 1000));
	}
}

void ModeClear::SetButton() {

	//�{�^���I����UI 
	{
		auto targetUI = _buttonServer->GetSelectUI();

		targetUI->SetHandle(ResourceServer::LoadGraph("res/UI/Result/ui_target_01.png"));
		targetUI->SetSize(Vector2(384.f * SCREEN_WIDTH_MAG, 64.f * SCREEN_HEIGHT_MAG));
		targetUI->SetAlpha(1.f);
		targetUI->AddAnimation(NEW OpacityAnimation(targetUI, -120, 0.6f));
	}

	//���̃X�e�[�W������Ȃ�
	//���̃X�e�[�W�ֈڍs����{�^��
	if(_clearData->_nextStageName.size() > 0)
	{
		//�摜�̐ݒ�
		SpriteText* titleButton = NEW SpriteText(
			ResourceServer::LoadGraph("res/UI/Result/ui_nextstage_01.png"),
			Transform2(Vector2(Vector2(3000.f * SCREEN_WIDTH_MAG, 824.f * SCREEN_HEIGHT_MAG))),
			Vector2(384.f * SCREEN_WIDTH_MAG, 64.f * SCREEN_HEIGHT_MAG)
		);
		//�A�j���[�V�����̐ݒ�
		titleButton->AddAnimation(NEW TransformAnimation(titleButton, 60, Transform2(Vector2(1344.f * SCREEN_WIDTH_MAG, 824.f * SCREEN_HEIGHT_MAG))));

		_buttonServer->AddButton(
			NEW Button(
				_buttonServer,

				[this]() {
					auto func = [this]() {
						// ���[�h�̍폜
						ModeServer::GetInstance()->Del(this);
						// ���̃��[�h��o�^
						ModeServer::GetInstance()->Add(NEW ModeGame(_clearData->_nextStageName), 1, "game");
					};
					// ���̃��[�h��o�^
					ModeBase* mode = NEW ModeColorOut(NEW ModeColorIn(60, true), func, 60);
					ModeServer::GetInstance()->Add(mode, 100, "Out");

					//���x�����������Ȃ��悤�A�{�^���T�[�o�[�̏��������Ȃ��ݒ�ɂ���
					_buttonServer->SetStep(ButtonServer::STEP::kConclude);
				},
				titleButton
					)
		);
	}
	//�Ȃ��Ȃ�
	//�^�C�g���ֈڍs����{�^��
	else {
		//�摜�̐ݒ�
		SpriteText* titleButton = NEW SpriteText(
			ResourceServer::LoadGraph("res/UI/Result/ui_totitle_01.png"),
			Transform2(Vector2(Vector2(3000.f * SCREEN_WIDTH_MAG, 824.f * SCREEN_HEIGHT_MAG))),
			Vector2(384.f * SCREEN_WIDTH_MAG, 64.f * SCREEN_HEIGHT_MAG)
		);
		//�A�j���[�V�����̐ݒ�
		titleButton->AddAnimation(NEW TransformAnimation(titleButton, 60, Transform2(Vector2(1344.f * SCREEN_WIDTH_MAG, 824.f * SCREEN_HEIGHT_MAG))));

		_buttonServer->AddButton(
			NEW Button(
				_buttonServer,

				[this]() {
					auto func = [this]() {
						// ���[�h�̍폜
						ModeServer::GetInstance()->Del(this);
						// ���̃��[�h��o�^
						ModeServer::GetInstance()->Add(NEW ModeTitle(), 1, "title");
					};
					// ���̃��[�h��o�^
					ModeBase* mode = NEW ModeColorOut(NEW ModeColorIn(60, true), func, 60);
					ModeServer::GetInstance()->Add(mode, 100, "Out");

					//���x�����������Ȃ��悤�A�{�^���T�[�o�[�̏��������Ȃ��ݒ�ɂ���
					_buttonServer->SetStep(ButtonServer::STEP::kConclude);
				},
				titleButton
					)
		);
	}

	//�����X�e�[�W�����v���C����{�^��
	{
		//�摜�̐ݒ�
		SpriteText* titleButton = NEW SpriteText(
			ResourceServer::LoadGraph("res/UI/Result/ui_playagain_01.png"),
			Transform2(Vector2(Vector2(3000.f * SCREEN_WIDTH_MAG, 896.f * SCREEN_HEIGHT_MAG))),
			Vector2(384.f * SCREEN_WIDTH_MAG, 64.f * SCREEN_HEIGHT_MAG)
		);
		//�A�j���[�V�����̐ݒ�
		titleButton->AddAnimation(NEW TransformAnimation(titleButton, 60, Transform2(Vector2(1344.f * SCREEN_WIDTH_MAG, 896.f * SCREEN_HEIGHT_MAG))));

		_buttonServer->AddButton(
			NEW Button(
				_buttonServer,

				[this]() {
					auto func = [this]() {
						// ���[�h�̍폜
						ModeServer::GetInstance()->Del(this);
						// ���̃��[�h��o�^
						ModeServer::GetInstance()->Add(NEW ModeGame(_clearData->_stageName), 1, "game");
					};
					// ���̃��[�h��o�^
					ModeBase* mode = NEW ModeColorOut(NEW ModeColorIn(60, true), func, 60);
					ModeServer::GetInstance()->Add(mode, 100, "Out");

					//���x�����������Ȃ��悤�A�{�^���T�[�o�[�̏��������Ȃ��ݒ�ɂ���
					_buttonServer->SetStep(ButtonServer::STEP::kConclude);
				},
				titleButton
					)
		);
	}
	//�X�e�[�W�Z���N�g
	{
		SpriteText* button = NEW SpriteText(
			ResourceServer::LoadGraph("res/UI/Result/ui_stageselection_01.png"),
			Transform2(Vector2(3000.f * SCREEN_WIDTH_MAG, 970.f * SCREEN_HEIGHT_MAG)),
			Vector2(384.f * SCREEN_WIDTH_MAG, 64.f * SCREEN_HEIGHT_MAG)
		);
		button->AddAnimation(NEW TransformAnimation(button, 60, Transform2(Vector2(1344.f * SCREEN_WIDTH_MAG, 970.f * SCREEN_HEIGHT_MAG))));

		_buttonServer->AddButton(
			NEW Button(
				_buttonServer,

				[this]() {
					auto func = [this]() {
						// ���[�h�̍폜
						ModeServer::GetInstance()->Del(this);
						// ���̃��[�h��o�^
						ModeServer::GetInstance()->Add(NEW ModeStageSelect(), 100, "select");
						};
					// ���̃��[�h��o�^
					ModeBase* mode = NEW ModeColorOut(NEW ModeColorIn(60, true), func, 60);
					ModeServer::GetInstance()->Add(mode, 100, "Out");

					//���x�����������Ȃ��悤�A�{�^���T�[�o�[�̏��������Ȃ��ݒ�ɂ���
					_buttonServer->SetStep(ButtonServer::STEP::kConclude);
				},
				button
			)
		);

	}
	

	
}

