#pragma once
/*
	サブプログラマーが急病
	3月17日まで、連絡の取れない状態になった
	そのため、終わっていない仕事を請け負った。
	だが、サブプログラマーが残したUI用のコードが理解できず、あえなく自分で書くことになった
*/


class UI {
public:
	UI(){}
	virtual ~UI(){}

	virtual bool Process() = 0;
	virtual bool Draw() = 0;
};
