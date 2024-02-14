#pragma once
#include"ObjectBase.h"

class CharaBase :public ObjectBase{
public :
	CharaBase(class ObjectServer* server,std::string name);
	virtual ~CharaBase();

	bool Initialize()override;
	bool Terminate()override;
	bool Render()override;

	void FixPos(); //�L������I�u�W�F�N�g�̏d�Ȃ���C��

	ObjectBase* GetOnObject()const { return _onObj; }

	class CapsuleComponent* GetCapsuleComponent() { return _capsule; }
	class AnimationComponent* GetAnimationComponent() { return _anim; }

	//���炩���ߌ��߂Ă�����string�^�Ŕh����̃L�����N�^�[�N���X�̏�Ԃ�ύX����
	//���[�V�����f�[�^�ɂ��A�j���[�V�����̐؂�ւ��Ɏg�p����̂ŁA�K���e�h����Œ�`���� 
	virtual bool ChangeState(std::string stateName);

protected:

	bool	_isStand;		//�I�u�W�F�N�g�̏�ɗ����Ă��邩

	class AnimationComponent* _anim;
	class CapsuleComponent* _capsule;

	//����Ă���I�u�W�F�N�g�̃A�h���X���i�[ ����Ă���I�u�W�F�N�g���ړ����Ĉꏏ�Ɉړ�����Ƃ���
	ObjectBase* _onObj;

};

