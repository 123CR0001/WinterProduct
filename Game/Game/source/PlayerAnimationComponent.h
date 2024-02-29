#pragma once
#include"AnimationComponent.h"

class PlayerAnimationComponent :public AnimationComponent {
public:
	PlayerAnimationComponent(class Player* owner, int order = 1000);
	~PlayerAnimationComponent();

	bool Process()override;

private:
	class Player* _pOwner;
};