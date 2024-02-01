#pragma once
#include"PhysWorld.h"
#include"appframe.h"

//�Q�[�����ł̕������E�̏���

class PhysWorld {
public:
	PhysWorld(class ObjectServer* server);
	~PhysWorld();

	//�����蔻�茋�ʃf�[�^
	struct CollisionDetectionItem {
		Vector3D pushVec;		//
		float pushDistanceSqaure = 0.f;			//�����o������
		class ObjectBase* _object = nullptr;	//�d�Ȃ����I�u�W�F�N�g�̃A�h���X
		Vector3D hitPosition;
	};
	
	struct CollisionDetectionResult {
		bool isHit = false;
		CollisionDetectionItem item;
	};

	bool IsHear(class ObjectBase* object,Vector3D& pos);

	std::vector<class CapsuleComponent*>& GetCapsuleComponent() { return _capsules; }
	std::vector<class FrameComponent*>& GetFrameComponent() { return _frames; }
	std::vector<class SoundComponent*>& GetSoundComponent() { return _sounds; }
protected:
	class ObjectServer* _server;
	std::vector<class CapsuleComponent*>_capsules;	//�J�v�Z��
	std::vector<class FrameComponent*>_frames;			//���f���̃t���[��
	std::vector<class SoundComponent*>_sounds;			//�Q�[�����E�ł̉�(����͋�)
};