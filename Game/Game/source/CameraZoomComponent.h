#pragma once
#include"Component.h"

class CameraZoomComponent :public Component {
public:
	CameraZoomComponent(class CameraComponent* component,float changeMag,int maxFrameCnt);
	~CameraZoomComponent();

	bool Process()override;

private:

	//�ǂ̔{���ɂ��邩
	float _changeMag;

	//�ύX�O�̒l
	float _mag;

	//���t���[���ŃY�[�����I��邩
	int _maxFrameCnt;
	//���t���[���o�������L�^����
	int _frameCnt;

	//�Y�[��������J����
	class CameraComponent* _camera;
};