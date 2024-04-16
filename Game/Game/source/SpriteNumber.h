#pragma once
#include"SpriteText.h"
#include<array>

//右揃えで、数字を描画する
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

	std::array<int, 10>_handles;	//数字の描画用ハンドル

	int _number;					//描画する数字の値

	//描画する桁数
	//10を4桁で表示したいなど
	//0010
	int _drawDigit;
};