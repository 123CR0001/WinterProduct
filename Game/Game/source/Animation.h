#pragma once
#include"appframe.h"
#include<functional>

class Animation {
public:
	Animation(class SpriteText* text, int frame);
	virtual ~Animation();

	virtual bool Process();

	//アニメーションを逆再生
	void Reverse() {
		_isEnd = false; _isReverse = true;
	}

	//アニメーションが終了したら、する処理を設定
	void SetFunc(std::function<void()>func) { _func = func; }

	//アニメーションが終了したか
	bool IsEnd()const { return _isEnd; }

	//アニメーションを終了
	void SkipEnd() {

		if (!_isReverse) {
			_frameCnt = _frame;
		}
		else {
			_frameCnt = 0;
		}
		_isEnd = true;
	}

protected:
	class SpriteText* _text;		//アニメーションをするSpriteText

	int _frameCnt;					//フレームカウント

	const int _frame;				//アニメーションの最大フレーム

	float _rate;					//アニメーションの進行度

	bool _isReverse;				//逆再生か	

	bool _isEnd;					//アニメーションが終了したか

	std::function<void()>_func;		//アニメーションが終了したら、する処理

};