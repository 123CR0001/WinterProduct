#pragma once
#include"Component.h"
#include"PhysWorld.h"

class CapsuleComponent :public Component {
public:
	CapsuleComponent(ObjectBase* owner, int order = 100);
	virtual ~CapsuleComponent();

	void SetMember(float length, float radius, Vector3D diff = Vector3D(0.f, 0.f, 0.f)) { _length = length; _radius = radius; }

	void AddSkipName(const char* name) { _skipName.emplace_back(name); }

	Capsule GetCapsule()const;

	bool Process()override;

	PhysWorld::CollisionDetectionResult GetOverlapResult();
	PhysWorld::CollisionDetectionResult GetEventOverlapResult()const { return _result; };

protected:
	float _length;	//�J�v�Z���̒���
	float _radius;	//�J�v�Z���̔��a
	Vector3D _diff;	//�I�u�W�F�N�g�̈ʒu����ǂꂾ������Ă��邩

	PhysWorld::CollisionDetectionResult _result;

	//�����o�����������Ȃ��L�����̖��O��o�^
	std::vector<std::string>_skipName;
};