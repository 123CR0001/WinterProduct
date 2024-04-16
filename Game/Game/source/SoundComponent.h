#pragma once
#include"Component.h"
#include"appframe.h"

class SoundComponent:public Component {
public:
	SoundComponent(class ObjectBase* owner, float volumeSize = 10.f);
	SoundComponent(class ObjectBase* owner,const Vector3& pos,float volumeSize = 10.f);
	~SoundComponent();

	bool Process()override;

	const Vector3& GetPos() const{ return _pos; }
	float GetVolumeSize() const{ return _volumeSize; }

	bool IsHear(const Vector3& pos);

protected:
	float _volumeSize;		//âπÇ™ï∑Ç±Ç¶ÇÈîÕàÕ
	Vector3 _pos;			//âπÇÃî≠ê∂à íu

};