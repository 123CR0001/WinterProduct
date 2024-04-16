#pragma once
#include"ObjectBase.h"

class CharaBase :public ObjectBase{
public :
	CharaBase(class ObjectServer* server,std::string name);
	virtual ~CharaBase();

	bool Initialize()override;
	bool Terminate()override;
	bool Render()override;

	class MoveComponent* GetMoveComponent()const { return _moveCom; }

	//���炩���ߌ��߂Ă�����string�^�Ŕh����̃L�����N�^�[�N���X�̏�Ԃ�ύX����
	//���[�V�����f�[�^�ɂ��A�j���[�V�����̐؂�ւ��Ɏg�p����̂ŁA�K���e�h����Œ�`���� 
	virtual bool ChangeState(std::string stateName);

protected:

	class MoveComponent* _moveCom;		//�ړ��R���|�[�l���g	

};

