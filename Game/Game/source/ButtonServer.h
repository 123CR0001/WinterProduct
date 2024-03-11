#pragma once
#include"ApplicationMain.h"
#include<functional>

class ButtonServer {
public:
	ButtonServer();
	~ButtonServer();

	bool Process();
	bool Draw();

	void AddButton(class Button* button);
	void DeleteButton(class Button* button);

	int GetSelectNum()const { return _selectNum; }
	int GetTrgSelectNum()const { return _trgSelectNum; }
	int GetRelSelectNum()const { return _relSelectNum; }

	class SpriteText* GetSelectUI()const { return _selectUI; }
private:
	int _selectNum;

	//ボタンから離れた時の処理とボタンに触れた時の処理をするかの比較用変数

	/*
		if(_trgSelectNum > 0) {
			_buttons->GetSpriteText()->AddAnimation(TransformAnimation()){}
		}
	*/
	int _oldSelectNum;
	int _trgSelectNum;
	int _relSelectNum;

	std::vector<class Button*>_buttons;

	class SpriteText* _selectUI;

	enum class STEP {
		kIntroduct,
		kProcess,
		kConclude
	}_step;

};