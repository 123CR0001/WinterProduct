#include"SpriteTextFlipAnimation.h"
SpriteTextFlipAnimation::SpriteTextFlipAnimation(int flipInterval, bool isLoop)
	:SpriteText()
	, _flipInterval(flipInterval)
	, _isReverse(false)
	, _isPlayAnimation(true)
	, _isLoop(isLoop)
	, _frameCnt(0)
{

}

SpriteTextFlipAnimation::~SpriteTextFlipAnimation() {}

void SpriteTextFlipAnimation::Draw(MATRIX mView) {

	int frameCnt = abs(_frameCnt);

	SetHandle(_handles[abs(frameCnt) / _flipInterval % _handles.size()]);

	//_isLoop��true�Ȃ�A�A�j���[�V���������[�v������
	if (_isPlayAnimation) {
		if (!_isReverse) {
			if ((_handles.size() - 1 > _frameCnt / _flipInterval) || _isLoop) {
				_frameCnt++;
			}
		}
		else {
			if (_frameCnt > 0 || _isLoop) {
				_frameCnt--;
			}
		}
	}

	//int�̏��ʂ𒴂����Ȃ��悤�ɂ���(�v�Z���N�͎���)
	if (frameCnt == INT_MAX) {
		_frameCnt = 0;
	}

	SpriteText::Draw();
}