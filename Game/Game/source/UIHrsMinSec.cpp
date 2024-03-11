#include"UIHrsMinSec.h"
#include"SpriteNumber.h"
#include"appframe.h"
#include"ApplicationMain.h"

//�R�����̕`��T�C�Y
constexpr float COLON_WIDTH = 10.f;
constexpr float COLON_HEIGHT = 35.f;

//�����̕`��T�C�Y
constexpr float NUMBER_WIDTH = 23.f;
constexpr float NUMBER_HEIGHT = 35.f;

constexpr int NUMBER_DIGIT = 2;

UIHrsMinSec::UIHrsMinSec(int frame,int secondTime)
	:_spriteColon(NEW SpriteText())
	,_secondTime(secondTime)
	,_frame(frame)
	,_frameCnt(0)
{


	int screenWidth = ApplicationMain::GetInstance()->DispSizeW();
	int screenHeight = ApplicationMain::GetInstance()->DispSizeH();

	//��ʂ̍ő剡��1920�ƌ��݂̉�ʉ����Ƃ̔䗦
	float rateW = static_cast<float>(screenWidth) / static_cast<float>(MAX_SCREEN_WIDTH);
	//��ʂ̍ő�c��1080�ƌ��݂̉�ʏc���Ƃ̔䗦
	float rateH = static_cast<float>(screenHeight) / static_cast<float>(MAX_SCREEN_HEIGHT);

	//��ʂ��ő�T�C�Y�̎��̕`��T�C�Y * ��L�̔䗦 = ��ʂ̑傫���ɓK�����`��T�C�Y�ɂ���

	_spriteColon->SetHandle(LoadGraph("res/UI/Result/ui_timer_02.png"));
	_spriteColon->SetSize(Vector2(COLON_WIDTH, COLON_HEIGHT));

	for (int a = 0; a < _spriteTimes.size(); a++) {

		_spriteTimes[a] = NEW SpriteNumber(0,NUMBER_DIGIT);
		//�摜�̃n���h��
		_spriteTimes[a]->LoadDivNumber("res/UI/Result/ui_timer_01.png", 5, 2, 46, 70);
		//�`��T�C�Y
		_spriteTimes[a]->SetSize(Vector2(NUMBER_WIDTH * rateW, NUMBER_HEIGHT * rateH));
		//�`��ʒu
		_spriteTimes[a]->SetPos(								//���Ԃƕ��A�b�͂Q�����`�悷��											//�R�����͈�񂾂�
			Vector2(screenWidth * 0.92f, screenHeight * 0.22f) - Vector2(NUMBER_WIDTH * rateW * static_cast<float>(NUMBER_DIGIT), 0.f) * a - Vector2(COLON_WIDTH * rateW, 0.f) * a
		);
	}

}

UIHrsMinSec::~UIHrsMinSec() {

	for (int a = 0; a < _spriteTimes.size(); a++) {
		delete _spriteTimes[a];
	}

	delete _spriteColon;
}

bool UIHrsMinSec::Process() {

	float rate = static_cast<float>(_frameCnt) / static_cast<float>(_frame);
	
	int secondTime = static_cast<int>(EasingLinear(0.f,static_cast<float>(_secondTime), rate));

	//���Ԃ̎Z�o
	int hrs = secondTime / 60 / 60;

	//���̎Z�o
	int min = (secondTime - hrs * 60 * 60) / 60;
	
	//�b�̎Z�o
	int sec = secondTime - (hrs * 60 * 60) - (min * 60);

	int nums[3] = { sec,min,hrs };

	//0:�b�@1:�� 2:����
	for (int a = 0; a < 3; a++) {
		_spriteTimes[a]->SetNumber(nums[a]);
	}

	if (_frameCnt < _frame) {
		_frameCnt++;
	}
	
	return true;
}

bool UIHrsMinSec::Draw() {

	int screenWidth = ApplicationMain::GetInstance()->DispSizeW();

	//��ʂ̍ő剡��1920�ƌ��݂̉�ʉ����Ƃ̔䗦
	float rateW = static_cast<float>(screenWidth) / static_cast<float>(MAX_SCREEN_WIDTH);

	for (int a = 0; a < _spriteTimes.size(); a++) {
		_spriteTimes[a]->Draw();

		//�R�����͎��Ԃ�`�悵�Ă���͕`�悵�Ȃ�
		if (a < _spriteTimes.size() - 1) {																							//���S���W����`�悵�Ă���̂ŁA/2
			_spriteColon->SetPos(_spriteTimes[a]->GetPos() - Vector2(NUMBER_WIDTH * rateW * static_cast<float>(NUMBER_DIGIT) , 0.f) + Vector2(COLON_WIDTH / 2.f, 0.f));
			_spriteColon->Draw();
		}
	}
	return true;
}