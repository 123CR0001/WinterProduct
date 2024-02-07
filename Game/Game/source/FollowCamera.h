#pragma once
#include"CameraComponent.h"

class FollowCamera : public CameraComponent {
public:
	FollowCamera(class ObjectBase* owner, int order = 9999);
	virtual ~FollowCamera();

	bool Initialize()override;
	bool Process()override;

protected:
	float _springConst;
	float _vDist;
	float _heightDist;
};