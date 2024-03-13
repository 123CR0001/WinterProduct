#pragma once
class LightsOut {
public:
	LightsOut(class ModeGame* game);
	~LightsOut();

	bool Process();

	void Use() { _state = STATE::kStart; }
	void Stop() { _state = STATE::kEnd; }

	bool IsUse() { return _state == STATE::kNone && _useTimes > 0; }

	void AddFrameCount() { _frameCnt += 120; }

private:
	class SpriteTextFlipAnimation* _timerBg;
	class SpriteTextFlipAnimation* _noise;
	class SpriteText* _hud;
	class UISecMiliSec* _timer;
	class ModeGame* _game;

	int _frameCnt;

	//使用できる回数
	int _useTimes;

	enum class STATE {
		kNone,
		kStart,
		kProcess,
		kEnd
	}_state;
};