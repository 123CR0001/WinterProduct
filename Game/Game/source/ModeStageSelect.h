#pragma once
#include"appframe.h"

class ModeStageSelect :public ModeBase {
public:

	ModeStageSelect();
	~ModeStageSelect();
	
	bool Initialize()override;
	bool Terminate()override;
	bool Process()override;
	bool Render()override;

private:
	//ボタンサーバー
	class ButtonServer* _buttonServer;

	//ステージの種類を選択したか
	bool _isStageTypeSelect;

	//選択しているステージの種類によって、描画する背景が違う
	std::array<class SpriteText*, 3>_bgs;

	//描画する背景の番号
	int _bgsNum;

	//飾りで描画する
	class SpriteText* _tag;

	//ステージの種類を選択してから、ステージを選択する
	//ステージの選択からステージの種類を選択に戻る際に、選択していたボタンに戻りたい
	//なので、選択したボタンの番号(ステージの種類)を保持する
	int _selectNum;
};