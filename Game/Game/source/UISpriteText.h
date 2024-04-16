#pragma once
#include"UI.h"

//受け取ったSpriteTextをただ、描画するだけのクラス
//背景だったり、UIの飾りで使用することを想定している
//フリップアニメーションがしたいなら、コンストラクタの引数にSpriteTextFlipAnimationのアドレスを渡せばいい

class UISpriteText :public UI {
public:
	UISpriteText(class SpriteText* text, int drawOrder = 100);
	virtual ~UISpriteText();

	const char* GetName() { return "SpriteText"; }

	bool Process()override;
	bool Draw()override;

private:
	class SpriteText* _text;			//描画するテキスト
};