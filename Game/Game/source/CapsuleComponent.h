#pragma once
#include"Component.h"
#include"PhysWorld.h"

class CapsuleComponent :public Component {
public:
	CapsuleComponent(ObjectBase* owner, int order = 100);
	virtual ~CapsuleComponent();

	void SetMember(float length, float radius, Vector3 diff = Vector3(0.f, 0.f, 0.f)) { _length = length; _radius = radius; }

	void AddSkipName(const char* name) { _skipNames.emplace_back(name); }

	Capsule GetCapsule()const;

	bool Process()override;

	PhysWorld::CollisionDetectionResult GetOverlapResult();
	PhysWorld::CollisionDetectionResult GetEventOverlapResult()const { return _result; };
	//
	class ObjectBase* GetBeginOverlap() const{
		if(_oldResult.item._object != _result.item._object) {
			return _result.item._object;
		}

		return nullptr;
	}

	class ObjectBase* GetEndOverlap() const{
		if(_oldResult.item._object != _result.item._object) {
			return _oldResult.item._object;
		}
		return nullptr;
	}
protected:
	float _length;	//�J�v�Z���̒���
	float _radius;	//�J�v�Z���̔��a
	Vector3 _diff;	//�I�u�W�F�N�g�̈ʒu����ǂꂾ������Ă��邩

	PhysWorld::CollisionDetectionResult _result;
	PhysWorld::CollisionDetectionResult _oldResult;

	//�����o�����������Ȃ��L�����̖��O��o�^
	std::vector<std::string>_skipNames;
};