#pragma once
#include"UI.h"
#include"appframe.h"

class UIMiniMap :public UI {
public:
	UIMiniMap(class ModeGame* game, int drawOrder = 100);
	~UIMiniMap();

	const char* GetName()override { return "MiniMap"; }

	bool Process()override;
	bool Draw()override;

private:

	class ModeGame* _game;

	//ミニマップの拡大縮小率
	float _mag;

	//全てのモデルの最大頂点のx,y,zの値から、最も大きいx値、y値、z値を参照して、マップ全体での最小頂点を算出する
	Vector3 _maxPos;

	//全てのモデルの最小頂点のx,y,zの値から、最も小さいx値、y値、z値を参照して、マップ全体での最小頂点を算出する
	Vector3 _minPos;

	//_maxPosと_mixPosを直線で結んだ際、その中間座標
	Vector3 _middlePos;

	//ミニマップを画面上に描画する位置
	int _x, _y;

	//_mapScreenに描画するミニマップの大きさ
	int  _w, _h;

	//画面に描画する円形上のマップの半径
	float _radius;

	//ミニマップの画像ハンドル
	int _mapTextHandle;

	//ミニマップ(キャラの位置など)を描画するスクリーンハンドル
	int _mapScreen;

	//_tempScreenHandleの一部分を切り抜くための、スクリーンハンドル		(円形や矩形などで、切り抜く)
	int _clipScreen;

	//_mapScreenと_clipScreenを合成した(_mapScreenを切り抜いた)スクリーンハンドル
	int _mixScreen;

	//ミニマップ上のプレイヤーの座標　この値を基準に描画位置を揃える
	Vector3 _mapPlayerPos;

	//ミニマップの拡大率
	float _zoom;

	//ミニマップ上に描画する円の色
	std::unordered_map<std::string, int>_identiColor;
};