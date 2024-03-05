#pragma once
#include"ObjectBase.h"

class Energy :public ObjectBase {
public:
	Energy(class ObjectServer* server);
	~Energy();

	bool Process()override;
	bool Render()override;

	//void SetJsonDataUE(nlohmann::json j)override;

private:
	const float _radius;
};