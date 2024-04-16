#pragma once
#include"ObjectBase.h"
#include<memory>
class Energy :public ObjectBase {
public:
	Energy(class ObjectServer* server);
	~Energy();

	bool Process()override;
	bool Render()override;

private:
	const float _radius;	//”¼Œa
};