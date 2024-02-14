#pragma once
#include"ObjectBase.h"

class Decoy :public ObjectBase {
public:
	Decoy(class Player* player);
	~Decoy();

	bool Initialize()override;
	bool Terminate()override;
	bool Process()override;

protected:
	class CapsuleComponent* _capsule;
	class AnimationComponent* _anim;

	int _hitCnt;
};