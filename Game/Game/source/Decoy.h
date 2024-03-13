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
	class CapsuleComponent* _capsule;

	//���Ԍo�߂ō폜���邽�߂̃J�E���g�ϐ��Ǝ��Ԍo�߂̖ڈ�
	int _elapsedFrame;

	const int _maxFrame;

	int _frameCnt;
};