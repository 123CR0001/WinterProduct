#pragma once
#include"appframe.h"
#include"Animation2D.h"

class ModeLightsOut :public ModeBase {
public:
	ModeLightsOut(class ModeGame* game);
	bool Initialize()override;
	bool Terminate()override;
	bool Process()override;
	bool Render()override;
protected:
	//画面に貼るエフェクト
	int _cg;
	//上記のエフェクトの透明度
	int _alpha;

	//画面に貼るエフェクトの上にかけるノイズのアニメーション
	Animation2D* _noiseAnim;

	int _textHud;

	//処理時間
	int _frameCnt;

	int _oldEnemyNum;

	//SEの再生間隔
	int _frameCntSE;

	class ModeGame* _game;
};