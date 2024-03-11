#pragma once
#include"Component.h"

class CameraZoomComponent :public Component {
public:
	CameraZoomComponent(class CameraComponent* component,float mag,int frameCnt);
	~CameraZoomComponent();

	bool Process()override;

private:

	//�ǂ̔{���ɂ��邩
	const float _changeMag;

	//�ύX�O�̒l
	const float _mag;
	

	//���t���[���ŃY�[�����I��邩
	const int _maxFrameCnt;
	//���t���[���o�������L�^����
	int _frameCnt;

	//�Y�[��������J����
	class CameraComponent* _camera;
};