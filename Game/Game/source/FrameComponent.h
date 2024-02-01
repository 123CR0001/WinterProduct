#pragma once
#include"Component.h"
#include"PhysWorld.h"

class FrameComponent :public Component {
public:
	FrameComponent(ObjectBase* owner, int order = 100);
	virtual ~FrameComponent();

	PhysWorld::CollisionDetectionResult GetOverlapResult();

	void DeletePhysWorldThis();
protected:

};