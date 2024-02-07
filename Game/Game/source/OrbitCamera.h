#pragma once
#include"CameraComponent.h"

class OrbitCamera :public CameraComponent {
public:
	OrbitCamera(class ObjectBase* owner, int order = 100);
	~OrbitCamera();

	bool Initialize()override { return true; }
	bool Process()override;

private:
	float _dist;
};