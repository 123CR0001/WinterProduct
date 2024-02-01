#pragma once
#include"Component.h"

class GravityComponent:public Component {
public:
	void Process(class ObjectBase* obj)override;
};
