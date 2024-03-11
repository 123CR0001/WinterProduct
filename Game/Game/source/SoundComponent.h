#pragma once
#include"Component.h"
#include"appframe.h"

class SoundComponent:public Component {
public:
	SoundComponent(class ObjectBase* owner, float volumeSize = 10.f);
	SoundComponent(class ObjectBase* owner,const Vector3D& pos,float volumeSize = 10.f);
	~SoundComponent();

	bool Process()override;

	const Vector3D& GetPos() const{ return _pos; }
	float GetVolumeSize() const{ return _volumeSize; }

	bool IsHear(const Vector3D& pos);

protected:
	float _volumeSize;
	Vector3D _pos;

};