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

	//時間経過で削除するためのカウント変数と時間経過の目安
	int _elapsedFrame;

	const int _maxFrame;

	int _frameCnt;
};