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
	class AIComponent* _AI;
	class CapsuleComponent* _capsule;
	class AnimationComponent* _anim;

	unsigned int _frameCnt;
	const unsigned int _maxFrame;

};