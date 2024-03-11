#pragma once
#include"appframe.h"
#include<functional>

class Animation {
public:
	Animation(class SpriteText* text, int frame);
	virtual ~Animation();

	virtual bool Process();

	void Reverse() { _isEnd = false; _isReverse = true; }

	bool IsEnd()const { return _isEnd; }

protected:
	class SpriteText* _text;

	//カウント
	int _frameCnt;

	//何カウントか
	const int _frame;

	//アニメーションの再生割合
	float _rate;

	//アニメーションを逆再生
	bool _isReverse;

	//アニメーションふぁ終了したか
	bool _isEnd;

	//アニメーションが終わったら、SpriteTextから削除するかの変数も欲しいかも？
	//bool _isEndDelete;

};