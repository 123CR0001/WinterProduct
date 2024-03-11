#pragma once
#include"Component.h"

class CountKillComboComponent :public Component {
public:
	CountKillComboComponent(class ObjectBase* owner);
	~CountKillComboComponent();

	bool Process()override;

private:
	//コンボをカウントするかの判断用のフレームカウント変数
	static int _frameCnt;

	//コンボ数
	static int _comboCnt;
};