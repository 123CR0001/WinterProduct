#pragma once
#include"CharaBase.h"

class Tracer :public CharaBase {
public:
	Tracer(class TracerSpawner* spawner);
	~Tracer();

	bool Initialize()override;
	bool Terminate()override;
	bool Process()override;

	class AIComponent* GetAI()const { return _AI; }
	
private:
	class AIComponent* _AI;					//AI�R���|�[�l���g
	class CapsuleComponent* _capsule;		//�J�v�Z���R���|�[�l���g
	class AnimationComponent* _anim;		//�A�j���[�V�����R���|�[�l���g

	unsigned int _frameCnt;					//�t���[���J�E���g	
	const unsigned int _maxFrame;			//�ő�t���[����

};