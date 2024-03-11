#pragma once
class LightsOut {
public:
	LightsOut(class ModeGame* game);
	~LightsOut();

	bool Process();

	void Use() { _state = STATE::kStart; }

	bool IsUse() { return _state == STATE::kNone && _useTimes > 0; }

private:
	class SpriteTextFlipAnimation* _timerBg;
	class SpriteTextFlipAnimation* _noise;
	class SpriteText* _hud;
	class UIHrsMinSec* _timer;
	class ModeGame* _game;

	int _frameCnt;

	//g—p‚Å‚«‚é‰ñ”
	int _useTimes;

	enum class STATE {
		kNone,
		kStart,
		kProcess,
		kEnd
	}_state;
};