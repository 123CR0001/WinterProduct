#pragma once
#include"CharaBase.h"

class Traser :public CharaBase {
public:
	Traser(class TraserSpawner* spawner);
	~Traser();

	bool Initialize()override;
	bool Terminate()override;
	bool Process()override;
	
private:
	class AIComponent* _AI;
	class CapsuleComponent* _capsule;
	class AnimationComponent* _anim;

	unsigned int _frameCnt;
	const unsigned int _maxFrame;

};