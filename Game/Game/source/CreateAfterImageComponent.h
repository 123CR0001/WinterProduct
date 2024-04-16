#pragma once
#include"Component.h"

class CreateAfterImageComponent :public Component {
public:
	CreateAfterImageComponent(class AnimationComponent* owner);
	~CreateAfterImageComponent();

	bool Process()override;

private:

	const int _maxCnt;					//残像を生成する間隔

	int _frameCnt;						//残像を生成する間隔のカウント

	class AnimationComponent* _anim;	//アニメーションコンポーネント
};