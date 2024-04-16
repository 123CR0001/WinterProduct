#pragma once
#include"Component.h"

class CameraZoomComponent :public Component {
public:
	CameraZoomComponent(class CameraComponent* component,float changeMag,int maxFrameCnt);
	~CameraZoomComponent();

	bool Process()override;

private:

	float _changeMag;						//�ύX����{��
	float _mag;								//���̔{��

	int _maxFrameCnt;						//���t���[���ŃY�[�����邩
	int _frameCnt;							//���t���[���o�������L�^����

	class CameraComponent* _camera;			//�Y�[��������J����
};