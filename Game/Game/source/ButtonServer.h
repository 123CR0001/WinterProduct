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

	std::vector<class Button*>& GetButtons() { return _buttons; }

	void SetSelectNum(int num) { _selectNum = num; }
	void SetSelectLimitNum(int num) { _selectLimitNum = num; }


	enum class STEP {
		kAnimation,
		kProcess,
	};
private:
	int _selectNum;
	int _selectLimitNum;

	//�{�^�����痣�ꂽ���̏����ƃ{�^���ɐG�ꂽ���̏��������邩�̔�r�p�ϐ�

	/*
		if(_trgSelectNum > 0) {
			_buttons->GetSpriteText()->AddAnimation(TransformAnimation()){}
		}
	*/
	int _oldSelectNum;
	int _trgSelectNum;
	int _relSelectNum;

	bool _isProcess;
	std::vector<class Button*>_buttons;
	std::vector<class Button*>_addButtons;
	std::vector<class Button*>_deleteButtons;

	class SpriteText* _selectUI;

	STEP _step;

public:
	STEP GetStep()const { return _step; }
};