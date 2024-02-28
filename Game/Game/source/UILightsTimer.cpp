
#include "ApplicationMain.h"
#include "UILightsTimer.h"
#include "Timer.h"

UILightsTimer::UILightsTimer() {

	int cgFrame[FRAME_NUM];
	int cgNum[TIME_NUM];

	_mCg["colon"].push_back( res::LoadGraph("res/UI/Game/Timer/ui_timer_02.png"));
	 res::LoadDivGraph("res/UI/Game/Timer/ui_timerbg_01.png",5,1,5,600,150, cgFrame);
	 res::LoadDivGraph("res/UI/Game/Timer/ui_timer_01.png", 10, 5, 2, 46, 70, cgNum);

	 for (int i = 0; i < FRAME_NUM; i++) {
		 _mCg["frame"].push_back(cgFrame[i]);
	 }
	 for (int i = 0; i < TIME_NUM; i++) {
		 _mCg["number"].push_back(cgNum[i]);
	 }

	_timer = new Timer();
	_bFrameOpen = false;
	_animCnt = 0;
	_frameCnt = 1;
	_motCnt = 0;
}

UILightsTimer::~UILightsTimer() {
	// ���������
	delete _timer;
	_mCg.clear();
	_mDigit.clear();
}

void UILightsTimer::Process() {
	// �^�C�}�[�̏���
	_timer->Process();
	
	if (_bFrameOpen) { AnimProcess(); }

	if (OpenFrame() && _timer->CheckTimeUnit() == "mSecond") {
		int nowTime = _timer->GetTimeMS();
		_mDigit["sec1"] = nowTime / 10000;
		_mDigit["sec2"] = nowTime / 1000 % 10;
		_mDigit["mSec1"] = nowTime / 100 % 10;
		_mDigit["mSec2"] = nowTime /10 % 10;
	}

}

void UILightsTimer::Render() {

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
	int x = ConvertX(_x);
	int y = ConvertY(_y);
	int w = ConvertX(_x) + ConvertX(_w);
	int h = ConvertY(_y) + ConvertY(_h);
	// �^�C�}�[�̔w�i
	DrawExtendGraph(x, y, w, h, _mCg["frame"][_motCnt], TRUE);

	if (OpenFrame()) {
		x += ConvertX(230);
		y += ConvertY(30);
		w = x + ConvertX(46);
		h = y + ConvertY(70);
		// �^�C�}�[�̐���
		DrawExtendGraph(x, y, w, h, _mCg["number"][_mDigit["sec1"]], TRUE);
		x += ConvertX(46);
		w = x + ConvertX(46);
		DrawExtendGraph(x, y, w, h, _mCg["number"][_mDigit["sec2"]], TRUE);
		x += ConvertX(46);
		w = x + ConvertX(15);
		DrawExtendGraph(x, y, w, h, res::LoadGraph("res/UI/Game/Timer/ui_timer_02.png"), TRUE);
		x += ConvertX(15);
		w = x + ConvertX(46);
		DrawExtendGraph(x, y, w, h, _mCg["number"][_mDigit["mSec1"]], TRUE);
		x += ConvertX(46);
		w = x + ConvertX(46);
		DrawExtendGraph(x, y, w, h, _mCg["number"][_mDigit["mSec2"]], TRUE);
	}
}

int UILightsTimer::Selected() {
	// �t���[���̃I�[�v���A�N���[�Y
	_bFrameOpen = !_bFrameOpen;

	return 0;
}

bool UILightsTimer::OpenFrame() {
	if (_motCnt == 4) {
		_bFrameOpen = false;
		return true;
	}
	return false;
}

void UILightsTimer::AnimProcess() {
	_animCnt++;
	if (_animCnt > _frameCnt) {
		_animCnt = 0;
		_motCnt++;
	}
	
	if (OpenFrame()) {
		// �^�C�}�[�̐ݒ�
		_timer->SetTimeMS(10000); 
		_timer->Start(); 
	}
}