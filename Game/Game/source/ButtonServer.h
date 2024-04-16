#pragma once
#include"ApplicationMain.h"
#include<functional>

class ButtonServer {
public:
	ButtonServer();
	~ButtonServer();

	bool Process();
	bool Draw();

	//ボタンを追加
	void AddButton(class Button* button);

	//ボタンを削除
	void DeleteButton(class Button* button);

	//選択UIのテキストを取得
	class SpriteText* GetSelectUI()const { return _selectUI; }

	//ボタンのリストを取得
	std::vector<class Button*>& GetButtons() { return _buttons; }

	//選択しているボタンの番号を設定
	void SetSelectNum(int num) { _selectNum = num; }

	//選択しているボタンの番号を取得
	int GetSelectNum()const { return _selectNum; }

	enum class STEP {
		kAnimation,		//アニメーション処理
		kProcess,		//選択処理や選択の変更
		kConclude		//終了処理	
	};

	//処理ステップを取得
	STEP GetStep()const { return _step; }

	//処理ステップを設定
	void SetStep(STEP step) { _step = step; }
private:
	int _selectNum;										//選択しているボタンの番号

	std::vector<class Button*>_buttons;					
	std::vector<class Button*>_addButtons;
	std::vector<class Button*>_deleteButtons;

	class SpriteText* _selectUI;						//選択しているボタンのUI

	STEP _step;											//処理ステップ

};