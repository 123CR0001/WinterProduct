#pragma once
/*
	�T�u�v���O���}�[���}�a
	3��17���܂ŁA�A���̎��Ȃ���ԂɂȂ���
	���̂��߁A�I����Ă��Ȃ��d���𐿂��������B
	�����A�T�u�v���O���}�[���c����UI�p�̃R�[�h�������ł����A�����Ȃ������ŏ������ƂɂȂ���
*/


class UI {
public:
	UI(){}
	virtual ~UI(){}

	virtual bool Process() = 0;
	virtual bool Draw() = 0;
};
