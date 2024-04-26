#pragma once
#include"appframe.h"

class ModeStageSelect :public ModeBase {
public:

	ModeStageSelect();
	~ModeStageSelect();
	
	bool Initialize()override;
	bool Terminate()override;
	bool Process()override;
	bool Render()override;

private:
	//�{�^���T�[�o�[
	class ButtonServer* _buttonServer;

	//�X�e�[�W�̎�ނ�I��������
	bool _isStageTypeSelect;

	//�I�����Ă���X�e�[�W�̎�ނɂ���āA�`�悷��w�i���Ⴄ
	std::array<class SpriteText*, 3>_bgs;

	//�`�悷��w�i�̔ԍ�
	int _bgsNum;

	//����ŕ`�悷��
	class SpriteText* _tag;

	//�X�e�[�W�̎�ނ�I�����Ă���A�X�e�[�W��I������
	//�X�e�[�W�̑I������X�e�[�W�̎�ނ�I���ɖ߂�ۂɁA�I�����Ă����{�^���ɖ߂肽��
	//�Ȃ̂ŁA�I�������{�^���̔ԍ�(�X�e�[�W�̎��)��ێ�����
	int _selectNum;
};