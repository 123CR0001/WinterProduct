#pragma once
#include"UI.h"

//�󂯎����SpriteText�������A�`�悷�邾���̃N���X
//�w�i��������AUI�̏���Ŏg�p���邱�Ƃ�z�肵�Ă���
//�t���b�v�A�j���[�V�������������Ȃ�A�R���X�g���N�^�̈�����SpriteTextFlipAnimation�̃A�h���X��n���΂���

class UISpriteText :public UI {
public:
	UISpriteText(class SpriteText* text, int drawOrder = 100);
	virtual ~UISpriteText();

	const char* GetName() { return "SpriteText"; }

	bool Process()override;
	bool Draw()override;

private:
	class SpriteText* _text;			//�`�悷��e�L�X�g
};