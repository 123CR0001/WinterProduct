#pragma once
#include"Component.h"

class CountClearTimeComponent :public Component {
public:
	CountClearTimeComponent(class ObjectBase* owner);
	~CountClearTimeComponent();

	bool Process()override;

private:
	int _frameCnt;
};