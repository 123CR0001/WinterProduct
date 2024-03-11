#pragma once
#include"UI.h"

//�󂯎����SpriteText�������A�`�悷�邾���̃N���X
//�w�i��������AUI�̏���Ŏg�p���邱�Ƃ�z�肵�Ă���
//�t���b�v�A�j���[�V�������������Ȃ�A�R���X�g���N�^�̈�����SpriteTextFlipAnimation�̃A�h���X��n���΂���

class UISpriteText :public UI {
public:
	UISpriteText(class SpriteText* text);
	virtual ~UISpriteText();

	bool Process()override;
	bool Draw()override;

private:
	class SpriteText* _text;

};