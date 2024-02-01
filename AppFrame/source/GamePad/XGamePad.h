#pragma once
#include"../Math/mymath.h"
#include"DxLib.h"
//struct XINPUT_STATE
//{
//	unsigned char	Buttons[16];	// ボタン１６個( 添字には XINPUT_BUTTON_DPAD_UP 等を使用する、
//	//			0:押されていない  1:押されている )
//	unsigned char	LeftTrigger;	// 左トリガー( 0～255 )
//	unsigned char	RightTrigger;	// 右トリガー( 0～255 )
//	short		ThumbLX;	// 左スティックの横軸値( -32768 ～ 32767 )
//	short		ThumbLY;	// 左スティックの縦軸値( -32768 ～ 32767 )
//	short		ThumbRX;	// 右スティックの横軸値( -32768 ～ 32767 )
//	short		ThumbRY;	// 右スティックの縦軸値( -32768 ～ 32767 )
//};

// なんか10と11だけ抜けてる
//#define XINPUT_BUTTON_DPAD_UP		(0)	// デジタル方向ボタン上
//#define XINPUT_BUTTON_DPAD_DOWN		(1)	// デジタル方向ボタン下
//#define XINPUT_BUTTON_DPAD_LEFT		(2)	// デジタル方向ボタン左
//#define XINPUT_BUTTON_DPAD_RIGHT	(3)	// デジタル方向ボタン右
//#define XINPUT_BUTTON_START		(4)	// STARTボタン
//#define XINPUT_BUTTON_BACK		(5)	// BACKボタン
//#define XINPUT_BUTTON_LEFT_THUMB	(6)	// 左スティック押し込み
//#define XINPUT_BUTTON_RIGHT_THUMB	(7)	// 右スティック押し込み
//#define XINPUT_BUTTON_LEFT_SHOULDER	(8)	// LBボタン
//#define XINPUT_BUTTON_RIGHT_SHOULDER	(9)	// RBボタン
//#define XINPUT_BUTTON_A			(12)	// Aボタン
//#define XINPUT_BUTTON_B			(13)	// Bボタン
//#define XINPUT_BUTTON_X			(14)	// Xボタン
//#define XINPUT_BUTTON_Y			(15)	// Yボタン

namespace {
	constexpr unsigned short INPUT_DPAD_UP =			0b0000000000000001;		 //1			
	constexpr unsigned short INPUT_DPAD_DOWN =			0b0000000000000010;		 //2		
	constexpr unsigned short INPUT__DPAD_LEFT =			0b0000000000000100;		 //4		
	constexpr unsigned short INPUT__DPAD_RIGHT =		0b0000000000001000;		 //8
	constexpr unsigned short INPUT_START =				0b0000000000010000;		 //16
	constexpr unsigned short INPUT_BACK =				0b0000000000100000;		 //32
	constexpr unsigned short INPUT_LEFT_THUMB =			0b0000000001000000;		 //64
	constexpr unsigned short INPUT_RIGHT_THUMB =		0b0000000010000000;		 //128
	constexpr unsigned short INPUT_LEFT_SHOULDER =		0b0000000100000000;		 //256
	constexpr unsigned short INPUT_RIGHT_SHOULDER =		0b0000001000000000;		 //512
	constexpr unsigned short INPUT_LEFT2_SHOULDER =		0b0000010000000000;		 //1024
	constexpr unsigned short INPUT_RIGHT2_SHOULDER =	0b0000100000000000;		 //2048
	constexpr unsigned short INPUT_A =					0b0001000000000000;		 //4096
	constexpr unsigned short INPUT_B =					0b0010000000000000;		 //8192
	constexpr unsigned short INPUT_X =					0b0100000000000000;		 //16384
	constexpr unsigned short INPUT_Y =					0b1000000000000000;		 //32768

	constexpr unsigned short STICK_DEPTH = 20000;

	struct STICK {
		short x, y;
	};
}

class XGamePad {
public:
	XGamePad(unsigned char num);
	~XGamePad();
	virtual void Input();

	unsigned int GetKeyButton()const { return _key; }
	unsigned int GetTrgButton()const { return _trg; }
	unsigned int GetRelButton()const { return _rel; }

	unsigned char GetLeftTrigger()const { return _input.LeftTrigger; }
	unsigned char GetRightTrigger()const { return _input.RightTrigger; }

	STICK GetLeftStick()const {
		STICK stick = { _input.ThumbLX ,_input.ThumbLY };
		return stick;
	}

	STICK GetRightStick()const {
		STICK stick = { _input.ThumbRX ,_input.ThumbRY };
		return stick;
	}

	bool IsInputLeftStick()const { return _input.ThumbLX > STICK_DEPTH || _input.ThumbLX < -STICK_DEPTH || _input.ThumbLY > STICK_DEPTH || _input.ThumbLY < -STICK_DEPTH; }
	bool IsInputRightStick()const { return _input.ThumbRX > STICK_DEPTH || _input.ThumbRX < -STICK_DEPTH || _input.ThumbRY > STICK_DEPTH || _input.ThumbRY < -STICK_DEPTH;	}

	float GetLeftStickRad()const { return (float) (atan2(_input.ThumbLY, _input.ThumbLX)); }
	float GetRightStickRad()const { return (float) ( atan2(_input.ThumbRY, _input.ThumbRX)); }

protected:
	//更新するゲームパッドの種類
	unsigned char _padNum;

	//更新するゲームパッドの構造体
	//右のアナログスティックが使いたいから
	XINPUT_STATE _input;

	//入力の計算はbit演算で行う
	//XINPUT_STATEをトリガーとリリース分用意するよりもメモリのバイト数を減らせるため
	//欲しいのはボタンだけでスティック系はいらないのと
	// 	//unsigned char			ボタンの数				short		左右スティックの縦横軸分		XINPUT_STATEの使用メモリ
	//		1byte		×		(16 + 2)		+		2byte	×				4				=	26byte

	//XINPUT_STATEをトリガーとリリース分用意すると78byteになる
	//これをbit演算で使用するint型変数3個だと、
	//unsigned int				個数				XINPUT_STATEは一つは必要だから				合計使用メモリ 
	//		4byte		×		3		+				26byte					=		38byte
	//40byteの節約になる
	//定数を含めたら、使用メモリは増えてはいるがゲームパッドの情報が増えた時を考慮すると多分少なくなっている
	unsigned int _key;
	unsigned int _trg;
	unsigned int _rel;

};



