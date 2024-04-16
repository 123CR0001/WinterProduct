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
	class CapsuleComponent* _capsule;	//カプセルコンポーネント

	int _elapsedFrameCnt;		//時間経過でデコイを削除するためのカウント変数

	const int _maxFrame;		//上記のカウント変数がこの値に達したらデコイを削除する

	int _frameCnt;				//フレームカウント
};