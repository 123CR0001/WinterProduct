#pragma once
#include"ObjectBase.h"

class Decoy :public ObjectBase {
public:
	Decoy(class Player* player);
	~Decoy();

	bool Initialize()override;
	bool Terminate()override;
	bool Process()override;
	bool Render()override;

protected:
	class CapsuleComponent* _capsule;

	int _hitCnt;

	int _frameCnt;
};