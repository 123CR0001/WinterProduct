#pragma once
#include"ObjectBase.h"

class CharaBase :public ObjectBase{
public :
	CharaBase(class ObjectServer* server);
	virtual ~CharaBase();

	bool Initialize()override;
	bool Terminate()override;
	bool Render()override;

	float GetRadian()const { return _radian; }
	float GetCollSubY()const { return _colSubY; }

	void FixPos(); //�L������I�u�W�F�N�g�̏d�Ȃ���C��

	ObjectBase* GetOnObject()const { return _onObj; }
	Segment GetCapsuleSegment() {
		Vector3D under_pos(_pos);
		under_pos.y += _radian;
		Vector3D over_pos(under_pos);
		over_pos.y += _colSubY;
		return Segment(under_pos, over_pos);
	}

	class CapsuleComponent* GetCapsuleComponent() { return _capsule; }

	//���炩���ߌ��߂Ă�����string�^�Ŕh����̃L�����N�^�[�N���X�̏�Ԃ�ύX����
	//���[�V�����f�[�^�ɂ��A�j���[�V�����̐؂�ւ��Ɏg�p����̂ŁA�K���e�h����Œ�`���� 
	virtual bool ChangeState(std::string stateName);

protected:
	//�J�v�Z���̓����蔻��
	float _radian;
	float _colSubY;	// �R���W�������莞��Y�␳(���ʒu�j.

	bool	_isStand;		//�I�u�W�F�N�g�̏�ɗ����Ă��邩

	class AnimationComponent* _anim;
	class CapsuleComponent* _capsule;

	//����Ă���I�u�W�F�N�g�̃A�h���X���i�[ ����Ă���I�u�W�F�N�g���ړ����Ĉꏏ�Ɉړ�����Ƃ���
	ObjectBase* _onObj;

	//���[�V�����f�[�^�̉��Ԗڂ��g�p���邩(�L�����̎�ނ́ustring�v�ŁA���[�V�����͌X�̃N���X(�񋓌^�N���X)�Œ�`)
	unsigned int _motCnt;

};

