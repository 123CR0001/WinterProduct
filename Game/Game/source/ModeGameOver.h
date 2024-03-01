#pragma once
#include "appframe.h"
#include "ModeGame.h"

class ModeGameOver :public ModeBase {
	typedef ModeBase base;
	typedef ResourceServer res;
public:
	ModeGameOver(ModeGame* game);

	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();

public:
	std::string GetStage() { return _stage; }	// ステージ名を取得

protected:
	ModeGame* _game;	// ゲームモードへのポインタ
	std::string _stage;	// ステージ名

	class UIServer* _uiServer;	// UIサーバー

	const int MAX_OPTIONS = 2;	// 項目の最大数
	const int LOW_LAYER_VALUE = 10;		// レイヤー値（10）
	const int BASIC_LAYER_VALUE = 20;	// レイヤー値（20）
	const int HIGH_LAYER_VALUE = 30;	// レイヤー値（30）

	int _select;	// 選択中の項目

#define BG_NUM 6
	int _cgBg[BG_NUM];	// 背景
	int _animCnt;		// アニメーションカウンタ
	int _frameCnt;		// フレームカウンタ
	int _motCnt;		// モーションカウンタ
};