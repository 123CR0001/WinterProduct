#pragma once

class AIState {
public:
	AIState(class AIComponent* owner);
	virtual ~AIState();
	virtual bool Process() = 0;

	//��Ԃ̐؂�ւ��̍ۂɎ��N���X�ɂȂ����ꍇ�ɂ��鏈��
	virtual void OnEnter() = 0;

	//��Ԃ̐؂�ւ��̍ۂɎ��N���X����ʃN���X�ɑJ�ڂ���ꍇ�ɂ��鏈��
	virtual void OnExist() = 0;

	//�ŗL�̖��O��Ԃ�
	virtual const char* GetName() = 0;

protected:
	class AIComponent* _owner;
};
