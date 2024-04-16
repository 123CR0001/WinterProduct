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

	//�I�����ɌĂяo���֐������s
	void SelectFunc() { if(_selectFunc)_selectFunc(); }

	//�I�����ɌĂяo���֐���ݒ�
	void SetFunc(std::function<void()>func) { _selectFunc = func; }

	SpriteText* GetSpriteText() { return _text; }

	class ButtonServer* GetServer()const { return _server; }

private:
	SpriteText* _text;					//�I�����Ă���{�^���̃e�L�X�g

	class ButtonServer* _server;		//�{�^���T�[�o�[

	std::function<void()>_selectFunc;	//�I�����ɌĂяo���֐�
};