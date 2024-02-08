#pragma once
#include"Component.h"

class CreateAfterImageComponent :public Component {
public:
	CreateAfterImageComponent(class CharaBase* owner);
	~CreateAfterImageComponent();

	bool Process()override;

private:
	//残像を生成する間隔
	const int _maxCnt;

	//フレームをカウントする変数
	int _frameCnt;

	class CharaBase* _chara;
};