#include"XGamePad.h"
//コントローラーの肩部分のボタン(2番目)だけ別の変数だった(LeftTriggerとRightTrigger)
//LeftTriggerとRightTriggerを押し込んでから反応するようにしている
constexpr short TRIGGER_DEPTH = 255;


XGamePad::XGamePad(unsigned char num){
	_padNum = num;
	_key = 0;
	_trg = 0;
	_rel = 0;
	_input = { {0},0,0,0,0,0,0 };
}

XGamePad::~XGamePad() {

}

void XGamePad::Input() {

	XINPUT_STATE old_input = _input;
	GetJoypadXInputState(_padNum, &_input);

	//初期化
	//しないと前の情報が残ったままでいつかメモリアウト?する
	_key = 0;
	_trg = 0;
	_rel = 0;

	//ボタンの数
	//unsigned char は数値として利用でき、1Byteしかメモリを使用しないから
	unsigned char button_num = sizeof(_input.Buttons) / sizeof(_input.Buttons[0]);

	for (int a = 0; a < button_num; a++) {
		//XINPUT_STATEは10と11だけ入力に関与しないから計算はしない
		//ただし、2^10と2^11の値は後で使用する
		if (a == 10 || a == 11) { continue; }

		if(a == XINPUT_BUTTON_DPAD_UP ) {
			if(_input.Buttons[a] == 0 && _input.ThumbLY > 30000) {
				_input.Buttons[a] = 1;
			}
		}

		if(a == XINPUT_BUTTON_DPAD_DOWN) {
			if(_input.Buttons[a] == 0 && _input.ThumbLY < -30000) {
				_input.Buttons[a] = 1;
			}
		}

		if(a == XINPUT_BUTTON_DPAD_LEFT) {
			if(_input.Buttons[a] == 0 && _input.ThumbLX < -30000) {
				_input.Buttons[a] = 1;
			}
		}

		if(a == XINPUT_BUTTON_DPAD_RIGHT) {
			if(_input.Buttons[a] == 0 && _input.ThumbLX > 30000) {
				_input.Buttons[a] = 1;
			}
		}

		if (_input.Buttons[a]) {
			_key += 1 << a;
		}
		if ((_input.Buttons[a] ^ old_input.Buttons[a]) & ~old_input.Buttons[a]) {
			_trg += 1 << a;
		}
		if ((_input.Buttons[a] ^ old_input.Buttons[a]) & old_input.Buttons[a]) {
			_rel += 1 << a;
		}
	}

	//LeftTriggerとRightTriggerのキー、トリガー、リリースをどれぐらい押されたかを確認して、取得する
	//しなくてもいいかもしれないけど、一応やっておく
	//上記で使用しなかった2^10と2^11の値をここで使用する
	auto Trigger = [this](unsigned char trigger,unsigned char old_trigger, unsigned char exp) {
		if (trigger >= TRIGGER_DEPTH) {
			_key += 1 << exp;
		}
		if (trigger == TRIGGER_DEPTH) {
			if ((trigger ^ old_trigger) & ~old_trigger)
				_trg += 1 << exp;
		}
		if (old_trigger == TRIGGER_DEPTH) {
			if ((trigger ^ old_trigger) & old_trigger)
				_rel += 1 << exp;
		}
	};

	Trigger(_input.LeftTrigger,old_input.LeftTrigger,10);
	Trigger(_input.RightTrigger,old_input.RightTrigger,11);

}