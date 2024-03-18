#pragma once
#include"ObjectBase.h"

class EventBox :public ObjectBase {
public:
	EventBox(class ObjectServer* server);
	virtual ~EventBox();

	bool Initialize()override;
	bool Process()override;

protected:
	class FrameComponent* _frameCom;
};