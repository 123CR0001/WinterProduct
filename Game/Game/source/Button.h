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

	void SelectFunc() { if(_selectFunc)_selectFunc(); }
	void SetFunc(std::function<void()>func) { _selectFunc = func; }

	SpriteText* GetSpriteText() { return _text; }

	class ButtonServer* GetServer()const { return _server; }

private:
	SpriteText* _text;

	class ButtonServer* _server;

	//選択されたときの処理
	std::function<void()>_selectFunc;

};