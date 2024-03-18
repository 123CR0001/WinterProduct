#pragma once
/*
	�T�u�v���O���}�[���}�a
	3��17���܂ŁA�A���̎��Ȃ���ԂɂȂ���
	���̂��߁A�I����Ă��Ȃ��d���𐿂��������B
	�����A�T�u�v���O���}�[���c����UI�p�̃R�[�h�������ł����A�����Ȃ������ŏ������ƂɂȂ���
*/


class UI {
public:
	UI(int drawOrder = 100) 
		:_drawOrder(drawOrder)
	{}
	virtual ~UI(){}

	virtual const char* GetName() = 0;

	virtual bool Process() = 0;
	virtual bool Draw() = 0;

	int GetDrawOrder()const { return _drawOrder; }

protected:
	int _drawOrder;
};
