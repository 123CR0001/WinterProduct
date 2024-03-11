#pragma once
#include"SpriteText.h"
#include<array>

//�E�����ŁA������`�悷��
class SpriteNumber :public SpriteText {
public:
	SpriteNumber(int number, int digit = -1);
	~SpriteNumber();

	void Draw(MATRIX mView = MGetIdent())override;

	void LoadDivNumber(const char* fileName, int XNum, int YNum, int XSize, int YSize) {
		LoadDivGraph(fileName, 10, XNum, YNum, XSize, YSize, _handles.data());
	}

	void SetNumber(int number) { _number = number; }
	int GetNumber()const { return _number; }

private:
	//�����̕`��p�n���h��
	std::array<int, 10>_handles;

	//�`�悷�鐔���̒l
	int _number;

	//�`�悷�錅��
	//10��4���ŕ\���������Ȃ�
	//0010
	int _drawDigit;
};