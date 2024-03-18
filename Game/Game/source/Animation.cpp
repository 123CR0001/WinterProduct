#include"Animation.h"
#include"SpriteText.h"

Animation::Animation(SpriteText* text, int frame)
	:_text(text)
	, _frameCnt(0)
	, _frame(frame)
	, _rate(0.f)
	, _isReverse(false)
	,_isEnd(false)
	,_func(nullptr)
{

}

Animation::~Animation(){}

bool Animation::Process() {

	//�A�j���[�V�������I�����Ă��邩
	if (!_isEnd) {
		//�t���[���J�E���g�̍X�V

		if (!_isReverse) {
			_frameCnt++;

			//�A�j���[�V�����̏I��
			if (_frameCnt == _frame) {
				_isEnd = true;
				if (_func) { _func(); }
			}
		}
		//_isReverse��true�Ȃ�A�t�Đ�
		else {
			_frameCnt--;

			//�A�j���[�V�����̏I��
			if (_frameCnt == 0) {
				_isEnd = true;
				if (_func) { _func(); }
			}
		}
	}

	
	//��N���炢���u�����烁�����A�E�g���邩��
	if (_frameCnt == INT32_MAX) {
		_frameCnt = 0;
	}

	//�A�j���[�V�����̍Đ�������_frmaeCnt��_frame����Z�o		//_frame�����̐��̏ꍇ������̂ŁA��Βl�ɂ��Ă���
	_rate = static_cast<float>(_frameCnt) / static_cast<float>(abs(_frame));

	//_frame��0�ȉ��A���̐��������ꍇ�A�j���[�V�����̍Đ�������0.f~1.f�ŁA�����Đ�����悤�ɂ���
	if (_frame < 0) {
		_rate = fabsf(sinf(static_cast<float>(_rate * 2.f * PI)));
	}

	return true;
}

