#pragma once
#include"Component.h"
#include"appframe.h"

class CameraComponent :public Component {
public:
	CameraComponent(class ObjectBase* owner,int oder = 9999);
	virtual ~CameraComponent();

	virtual bool Initialize() = 0;
	bool Process()override;

	Vector3D GetPos()const { return _pos; }	
	void Swap() { _swapWidth = rand() % 30 + 30; _swapHeight = rand() % 30 + 30; }
protected:
	Vector3D _pos;

	int _swapWidth, _swapHeight;
};