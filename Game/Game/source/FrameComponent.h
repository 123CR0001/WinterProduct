#pragma once
#include"Component.h"
#include"PhysWorld.h"

class FrameComponent :public Component {
public:
	FrameComponent(ObjectBase* owner, int order = 100);
	virtual ~FrameComponent();

	bool Process()override;

	PhysWorld::CollisionDetectionResult GetOverlapResult();

	class ObjectBase* GetOverlap()const { return _result.item._object; }
	class ObjectBase* GetBeginOverlap() const {
		if(_oldResult.item._object != _result.item._object) {
			return _result.item._object;
		}

		return nullptr;
	}

	class ObjectBase* GetEndOverlap() const {
		if(_oldResult.item._object != _result.item._object) {
			return _oldResult.item._object;
		}
		return nullptr;
	}

	void DeletePhysWorldThis();
protected:
	PhysWorld::CollisionDetectionResult _result;		//Õ“Ë”»’èŒ‹‰Ê
	PhysWorld::CollisionDetectionResult _oldResult;		//‘O‰ñ‚ÌÕ“Ë”»’èŒ‹‰Ê
};