#pragma once
#include"CameraComponent.h"

class FollowCamera : public CameraComponent {
public:
	FollowCamera(class ObjectBase* owner, int order = 9999);
	virtual ~FollowCamera();

	bool Initialize()override;
	bool Process()override;

protected:

	float _springConst;						//�΂˒萔

	//�^�[�Q�b�g�Ƃ̒����I�ȋ����ł͂Ȃ��A��ӂƍ���
	float _widthDist;
	float _heightDist;

	Vector3 _velocity;							//�΂˒萔��p�����v�Z�ɕK�v�ȑ��x��ێ����邽�߂̕ϐ�
};