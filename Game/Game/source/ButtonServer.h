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

	class SpriteText* GetSelectUI()const { return _selectUI; }

	std::vector<class Button*>& GetButtons() { return _buttons; }

	void SetSelectNum(int num) { _selectNum = num; }
	void SetSelectLimitNum(int num) { _selectLimitNum = num; }


	enum class STEP {
		kAnimation,
		kProcess,
		kConclude
	};
private:
	int _selectNum;
	int _selectLimitNum;

	//ボタンから離れた時の処理とボタンに触れた時の処理をするかの比較用変数
	int _oldSelectNum;

	std::vector<class Button*>_buttons;
	std::vector<class Button*>_addButtons;
	std::vector<class Button*>_deleteButtons;

	class SpriteText* _selectUI;

	STEP _step;

public:
	STEP GetStep()const { return _step; }
	void SetStep(STEP step) { _step = step; }
};