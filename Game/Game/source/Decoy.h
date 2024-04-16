#pragma once
#include"ObjectBase.h"

class Decoy :public ObjectBase {
public:
	Decoy(class Player* player,float rad);
	~Decoy();

	bool Initialize()override;
	bool Terminate()override;
	bool Process()override;
	bool Render()override;

protected:
	class CapsuleComponent* _capsule;	//�J�v�Z���R���|�[�l���g

	int _elapsedFrameCnt;		//���Ԍo�߂Ńf�R�C���폜���邽�߂̃J�E���g�ϐ�

	const int _maxFrame;		//��L�̃J�E���g�ϐ������̒l�ɒB������f�R�C���폜����

	int _frameCnt;				//�t���[���J�E���g
};