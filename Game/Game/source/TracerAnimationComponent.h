#pragma once
#include"AnimationComponent.h"

class TracerAnimationComponent :public AnimationComponent {
public:
	TracerAnimationComponent(class Tracer* owner, int order = 1000);
	~TracerAnimationComponent();

	bool Process()override;

private:
	class Tracer* _tOwner;
};