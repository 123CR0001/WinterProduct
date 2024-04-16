#pragma once
#include"Component.h"
#include"appframe.h"

class CameraComponent :public Component {
public:
	CameraComponent(class ObjectBase* owner,int oder = 9999);
	virtual ~CameraComponent();

	virtual bool Initialize() = 0;
	bool Process()override;

	Vector3 GetPos()const { return _pos; }		//�J�����̈ʒu���擾
	Vector3 GetAngle()const { return _angle; }	//�J�����̊p�x���擾

	//�J������h�炷
	void Swap() { _swapWidth = rand() % 30 + 30; _swapHeight = rand() % 30 + 30; }

	//�^�[�Q�b�g�Ƃ̋����̔{����ݒ�
	void SetTargetDistmag(float mag) { _targetDistMag = mag; }

	//�^�[�Q�b�g�Ƃ̋����̔{�����擾
	const float GetTargetDistMag()const { return _targetDistMag; }
protected:
	Vector3 _pos;		//�J�����̈ʒu
	Vector3 _angle;		//�J�����̊p�x


	float _targetDistMag;		//�^�[�Q�b�g�Ƃ̋����̔{��

	int _swapWidth, _swapHeight;		//�h�ꕝ
};