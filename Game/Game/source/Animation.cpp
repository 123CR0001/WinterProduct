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

	//アニメーションが終了しているか
	if (!_isEnd) {
		//フレームカウントの更新

		if (!_isReverse) {
			_frameCnt++;

			//アニメーションの終了
			if (_frameCnt == _frame) {
				_isEnd = true;
				if (_func) { _func(); }
			}
		}
		//_isReverseがtrueなら、逆再生
		else {
			_frameCnt--;

			//アニメーションの終了
			if (_frameCnt == 0) {
				_isEnd = true;
				if (_func) { _func(); }
			}
		}
	}

	
	//一年ぐらい放置したらメモリアウトするから
	if (_frameCnt == INT32_MAX) {
		_frameCnt = 0;
	}

	//アニメーションの再生割合を_frmaeCntと_frameから算出		//_frameが負の数の場合もあるので、絶対値にしている
	_rate = static_cast<float>(_frameCnt) / static_cast<float>(abs(_frame));

	//_frameが0以下、負の数だった場合アニメーションの再生割合を0.f~1.fで、無限再生するようにする
	if (_frame < 0) {
		_rate = fabsf(sinf(static_cast<float>(_rate * 2.f * PI)));
	}

	return true;
}

