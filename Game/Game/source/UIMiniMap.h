#pragma once
#include"UI.h"
#include"appframe.h"

class UIMiniMap :public UI {
public:
	UIMiniMap(class ModeGame* game);
	~UIMiniMap();
	bool Process()override;
	bool Draw()override;

private:

	class ModeGame* _game;

	//�~�j�}�b�v�̊g��k����
	float _mag;

	//�S�Ẵ��f���̍ő咸�_��x,y,z�̒l����A�ł��傫��x�l�Ay�l�Az�l���Q�Ƃ��āA�}�b�v�S�̂ł̍ŏ����_���Z�o����
	Vector3D _maxPos;

	//�S�Ẵ��f���̍ŏ����_��x,y,z�̒l����A�ł�������x�l�Ay�l�Az�l���Q�Ƃ��āA�}�b�v�S�̂ł̍ŏ����_���Z�o����
	Vector3D _minPos;

	//_maxPos��_mixPos�𒼐��Ō��񂾍ہA���̒��ԍ��W
	Vector3D _middlePos;

	//�~�j�}�b�v����ʏ�ɕ`�悷��ʒu
	int _x, _y;

	//_mapScreen�ɕ`�悷��~�j�}�b�v�̑傫��
	int  _w, _h;

	//��ʂɕ`�悷��~�`��̃}�b�v�̔��a
	float _radius;

	//�~�j�}�b�v�̉摜�n���h��
	int _mapTextHandle;

	//�~�j�}�b�v(�L�����̈ʒu�Ȃ�)��`�悷��X�N���[���n���h��
	int _mapScreen;

	//_tempScreenHandle�̈ꕔ����؂蔲�����߂́A�X�N���[���n���h��		(�~�`���`�ȂǂŁA�؂蔲��)
	int _clipScreen;

	//_mapScreen��_clipScreen����������(_mapScreen��؂蔲����)�X�N���[���n���h��
	int _mixScreen;

	//�~�j�}�b�v��̃v���C���[�̍��W�@���̒l����ɕ`��ʒu�𑵂���
	Vector3D _mapPlayerPos;



	float _zoom;
};