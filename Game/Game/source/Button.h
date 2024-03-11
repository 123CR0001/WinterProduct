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

	void SelectFunc() { _selectFunc(); }

	SpriteText* GetSpriteText() { return _text; }

	class ButtonServer* GetServer()const { return _server; }

private:
	SpriteText* _text;

	class ButtonServer* _server;

	//‘I‘ğ‚³‚ê‚½‚Æ‚«‚Ìˆ—
	std::function<void()>_selectFunc;

};