#pragma once
#include"CameraComponent.h"

class FollowCamera : public CameraComponent {
public:
	FollowCamera(class ObjectBase* owner, int order = 9999);
	virtual ~FollowCamera();

	bool Initialize()override;
	bool Process()override;

protected:
	//�΂˒萔
	float _springConst;

	//�^�[�Q�b�g�Ƃ̒����I�ȋ����ł͂Ȃ��A��ӂƍ���
	float _widthDist;
	float _heightDist;

	//�΂˒萔��p�����v�Z�ɕK�v�ȑ��x��ێ����邽�߂̕ϐ�
	Vector3 _velocity;
};