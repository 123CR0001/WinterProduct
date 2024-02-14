#pragma once
#include"Component.h"
#include"PhysWorld.h"

class CapsuleComponent :public Component {
public:
	CapsuleComponent(ObjectBase* owner, int order = 100);
	virtual ~CapsuleComponent();

	void SetMember(float length, float radius, Vector3D diff = Vector3D(0.f, 0.f, 0.f)) { _length = length; _radius = radius; }

	Capsule GetCapsule()const;

	bool Process()override;

	PhysWorld::CollisionDetectionResult GetOverlapResult();

protected:
	float _length;	//カプセルの長さ
	float _radius;	//カプセルの半径
	Vector3D _diff;	//オブジェクトの位置からどれだけ離れているか
};