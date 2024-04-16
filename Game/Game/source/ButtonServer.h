#pragma once
#include"ApplicationMain.h"
#include<functional>

class ButtonServer {
public:
	ButtonServer();
	~ButtonServer();

	bool Process();
	bool Draw();

	//�{�^����ǉ�
	void AddButton(class Button* button);

	//�{�^�����폜
	void DeleteButton(class Button* button);

	//�I��UI�̃e�L�X�g���擾
	class SpriteText* GetSelectUI()const { return _selectUI; }

	//�{�^���̃��X�g���擾
	std::vector<class Button*>& GetButtons() { return _buttons; }

	//�I�����Ă���{�^���̔ԍ���ݒ�
	void SetSelectNum(int num) { _selectNum = num; }

	//�I�����Ă���{�^���̔ԍ����擾
	int GetSelectNum()const { return _selectNum; }

	enum class STEP {
		kAnimation,		//�A�j���[�V��������
		kProcess,		//�I��������I���̕ύX
		kConclude		//�I������	
	};

	//�����X�e�b�v���擾
	STEP GetStep()const { return _step; }

	//�����X�e�b�v��ݒ�
	void SetStep(STEP step) { _step = step; }
private:
	int _selectNum;										//�I�����Ă���{�^���̔ԍ�

	std::vector<class Button*>_buttons;					
	std::vector<class Button*>_addButtons;
	std::vector<class Button*>_deleteButtons;

	class SpriteText* _selectUI;						//�I�����Ă���{�^����UI

	STEP _step;											//�����X�e�b�v

};