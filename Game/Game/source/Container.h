#pragma once
#include"ObjectBase.h"
#include"appframe.h"

class Container :public ObjectBase {
public:
	Container(class ObjectServer* server);
	virtual ~Container();

	bool Initialize()override;
	bool Terminate()override;
	bool Process()override;
	bool Render()override;

	const char* GetName()override { return "Container"; }
protected:
	bool _is_fall;
	int _count;
};