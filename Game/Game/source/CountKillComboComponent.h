#pragma once
#include"Component.h"

class CountKillComboComponent :public Component {
public:
	CountKillComboComponent(class ObjectBase* owner);
	~CountKillComboComponent();

	bool Process()override;

private:
	//�R���{���J�E���g���邩�̔��f�p�̃t���[���J�E���g�ϐ�
	static int _frameCnt;

	//�R���{��
	static int _comboCnt;
};