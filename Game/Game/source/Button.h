#pragma once
#include"appframe.h"
#include"SpriteText.h"
#include<functional>
class Button {
public:
	Button(
		class ButtonServer* server
		,std::function<void()> selectFunc
		,SpriteText* text
	);
	virtual ~Button();

	bool Process();
	bool Draw();

	//選択時に呼び出す関数を実行
	void SelectFunc() { if(_selectFunc)_selectFunc(); }

	//選択時に呼び出す関数を設定
	void SetFunc(std::function<void()>func) { _selectFunc = func; }

	SpriteText* GetSpriteText() { return _text; }

	class ButtonServer* GetServer()const { return _server; }

private:
	SpriteText* _text;					//選択しているボタンのテキスト

	class ButtonServer* _server;		//ボタンサーバー

	std::function<void()>_selectFunc;	//選択時に呼び出す関数
};