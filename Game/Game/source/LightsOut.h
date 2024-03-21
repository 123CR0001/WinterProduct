#pragma once
class LightsOut {
public:
	LightsOut(class ModeGame* game);
	~LightsOut();

	bool Process();

	void Use() { if(_state==STATE::kNone)_state = STATE::kStart; }
	void Stop() { if(_state == STATE::kProcess)_state = STATE::kEnd; }

	bool IsUse() { return _state == STATE::kNone && _useTimes > 0; }

	void AddFrameCount() { _frameCnt += 120; }

private:
	class SpriteTextFlipAnimation* _timerBg;
	class SpriteTextFlipAnimation* _noise;
	class SpriteTextFlipAnimation* _timerEffect;
	class SpriteText* _hud;
	class UISecMiliSec* _timer;
	class ModeGame* _game;

	class CreateAfterImageComponent* _afterImageCom;

	int _frameCnt;

	//使用できる回数
	int _useTimes;

	//ライツアウトが使用可能になった時だけしたい処理があるため
	int _oldEnergyCount;

	enum class STATE {
		kNone,
		kStart,
		kProcess,
		kEnd
	}_state;
};