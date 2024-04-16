#pragma once
#include"AnimationComponent.h"

class CommonSoldierAnimationComponent :public AnimationComponent {
public:
	CommonSoldierAnimationComponent(class CommonSoldier* owner, int order = 1000);
	~CommonSoldierAnimationComponent();

	bool Process()override;

private:
	class CommonSoldier* _csOwner;		//オーナー
};