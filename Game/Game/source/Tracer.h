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
	class AIComponent* _AI;					//AIコンポーネント
	class CapsuleComponent* _capsule;		//カプセルコンポーネント
	class AnimationComponent* _anim;		//アニメーションコンポーネント

	unsigned int _frameCnt;					//フレームカウント	
	const unsigned int _maxFrame;			//最大フレーム数

};