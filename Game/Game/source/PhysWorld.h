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
		Vector3 pushVec;		//
		float pushDistanceSquare = 0.f;			//�����o������
		class ObjectBase* _object = nullptr;	//�d�Ȃ����I�u�W�F�N�g�̃A�h���X
		Vector3 hitPosition;
		Vector3 normal;
	};
	
	//�����蔻�茋��
	struct CollisionDetectionResult {
		bool isHit = false;
		CollisionDetectionItem item;
	};

	//SoundComponent�Ƃ̓����蔻��
	bool IsHear(class ObjectBase* object,Vector3* pos = nullptr);

	std::vector<class CapsuleComponent*>& GetCapsuleComponent() { return _capsules; }
	std::vector<class FrameComponent*>& GetFrameComponent() { return _frames; }
	std::vector<class SoundComponent*>& GetSoundComponent() { return _sounds; }
protected:
	class ObjectServer* _server;											//�I�u�W�F�N�g�T�[�o�[(�I�[�i�[)	
	std::vector<class CapsuleComponent*>_capsules;		//�J�v�Z��
	std::vector<class FrameComponent*>_frames;				//���f���̃t���[��
	std::vector<class SoundComponent*>_sounds;				//�Q�[�����E�ł̉�(����͋�)
};