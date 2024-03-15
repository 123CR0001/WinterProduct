#include"ModeGame.h"
#include"UIHrsMinSec.h"
#include"SpriteTextFlipAnimation.h"
#include"UI.h"
#include"MyUIServer.h"
#include"LightsOut.h"

#include"ModeColorIn.h"
#include"ModeColorOut.h"

#include"TimeLine.h"

#include"MyUIServer.h"
#include"UISpriteText.h"

#include"ObjectServer.h"
#include"Player.h"

#include"ApplicationMain.h"
#include"ApplicationGlobal.h"

#include"UISecMiliSec.h"

#include"CameraZoomComponent.h"
#include"CreateAfterImageComponent.h"
#include"AnimationComponent.h"
#include"CameraComponent.h"

#include"ObjectServer.h"

constexpr float MAG_WIDTH = static_cast<float>(SCREEN_WIDTH) / static_cast<float>(MAX_SCREEN_WIDTH);
constexpr float MAG_HEIGHT = static_cast<float>(SCREEN_HEIGHT) / static_cast<float>(MAX_SCREEN_HEIGHT);

LightsOut::LightsOut(ModeGame* game) 
	:_game(game)
	,_timerBg(NEW SpriteTextFlipAnimation(3,false))
	,_timer(NEW UISecMiliSec(Transform2(Vector2(359.f * MAG_WIDTH ,978.f * MAG_HEIGHT))))
	,_noise(NEW SpriteTextFlipAnimation(8, true))
	,_hud(NEW SpriteText())
	,_frameCnt(300)
	,_useTimes(1)
	,_state(STATE::kNone)
{

	int screenWidth = ApplicationMain::GetInstance()->DispSizeW();
	int screenHeight = ApplicationMain::GetInstance()->DispSizeH();

	_timerBg->Stop();
	_timerBg->LoadDivText("res/UI/Game/Timer/ui_timerbg_01.png", 5, 1, 5, 500, 150);
	_timerBg->SetPos(Vector2(359.f * MAG_WIDTH, 978.f * MAG_HEIGHT));
	_timerBg->SetSize(Vector2(600.f,150.f));

	_noise->LoadDivText("res/Effect/ui_nightscope_1.png", 6, 1, 6, 1920, 1080);
	_noise->SetSize(Vector2(screenWidth, screenHeight));
	_noise->SetPos(Vector2((float)screenWidth / 2,(float)screenHeight / 2));
	_noise->SetAlpha(0.f);

	_hud->SetHandle(ResourceServer::LoadGraph("res/hud_01.png"));
	_hud->SetSize(Vector2(screenWidth, screenHeight));
	_hud->SetPos(Vector2((float)screenWidth / 2, (float)screenHeight / 2));
	_hud->SetAlpha(0.f);

	_timer->SetIsDraw(false);

	//UI��ǉ�
	_game->GetUIServer()->AddUI(NEW UISpriteText(_noise));
	_game->GetUIServer()->AddUI(NEW UISpriteText(_hud));
	_game->GetUIServer()->AddUI(NEW UISpriteText(_timerBg));
	_game->GetUIServer()->AddUI(_timer);
}

LightsOut::~LightsOut(){

}

bool LightsOut::Process() {

	switch (_state) {
	case STATE::kNone:
		break;
	case STATE::kStart: {

		//�^�C�}�[�̔w�i�̃A�j���[�V�������n�߂�
		_timerBg->Play();

		//�c��̎g�p�񐔂����炷
		_useTimes--;

		//�^�C�}�[��30�t���[����ɕ`�悳���悤�ɂ���
		auto func = [this]() {
			_timer->SetIsDraw(true);
		};
		_game->GetTimeLine()->AddLine(30, func);

		_game->GetObjectServer()->GetPlayer()->SetIsLightsOut(true);

		//�t�F�[�h�C���A�E�g�𗘗p�������o
		ModeServer::GetInstance()->Add(NEW ModeColorOut(NEW ModeColorIn(10, true), nullptr, 10), 100, "Out");

		//
		_noise->SetAlpha(1.f);
		_hud->SetAlpha(1.f);

		//�v���C���[����c�����o�͂���悤�ɂ���
		NEW CreateAfterImageComponent(_game->GetObjectServer()->GetPlayer()->GetAnimationComponent());

		//Zoom�C��
		NEW CameraZoomComponent(_game->GetObjectServer()->GetPlayer()->GetCamera(), 0.6f, 60);

		//BGM�̍Đ�
		gGlobal._sndServer.Play("BGM_02");

		_state = STATE::kProcess;
		break;
	}

	case STATE::kProcess: {
		_frameCnt--;

		if (_frameCnt < 0) {
			
			_state = STATE::kEnd;
		}

		_timer->SetFrameCount(_frameCnt);

		if (_frameCnt % 60 == 0) {
			//SE
			gGlobal._sndServer.Get("SE_15")->Play();
		}

		break;
	}

	case STATE::kEnd: {
		
		//�A�j���[�V�������t�Đ�������
		_timerBg->Reverse();
		_timer->SetIsDraw(false);
	

		//�t�F�[�h�C���A�E�g�𗘗p�������o
		ModeServer::GetInstance()->Add(NEW ModeColorOut(NEW ModeColorIn(10, true), nullptr, 10), 100, "Out");

		if (_useTimes == 0) {
			//120�t���[����ɃQ�[���I�[�o�[���Q�[���N���A�����߂�
			auto func = [this]() {
				_game->SwitchOverOrClear();
			};

			_game->GetTimeLine()->AddLine(120, func);
		}
		_game->GetObjectServer()->GetPlayer()->SetIsLightsOut(false);

		_noise->SetAlpha(0.f);
		_hud->SetAlpha(0.f);

		//Zoom�A�E�g
		NEW CameraZoomComponent(_game->GetObjectServer()->GetPlayer()->GetCamera(), 1.f, 60);

		//BGM�̍Đ�
		gGlobal._sndServer.Play("BGM_01");
		_state = STATE::kNone;
		break;
	}
	
	}

	return true;
}