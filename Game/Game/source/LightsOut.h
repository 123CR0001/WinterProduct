#pragma once
class LightsOut {
public:
	LightsOut(class ModeGame* game);
	~LightsOut();

	bool Process();

	//状態が初期化時の時にだけ
	void Use() { if(_state==STATE::kNone)_state = STATE::kStart; }

	//状態が処理の時だけ
	void Stop() { if(_state == STATE::kProcess)_state = STATE::kEnd; }

	//ライツアウトを使用できる状態か
	bool IsUse() { return _state == STATE::kNone && _useTimes > 0; }

	//制限時間を加算
	void AddFrameCount() { _frameCnt += 120; }

private:
	class SpriteTextFlipAnimation* _timerBg;		//秒数の背景
	class SpriteTextFlipAnimation* _noise;			//エフェクト
	class SpriteTextFlipAnimation* _isUseLightsOut;	//ライツアウトが使用可能になったかを表示するUI 
	class SpriteText* _hud;							//HUD
	class UISecMiliSec* _timer;						//制限時間表示UI

	class ModeGame* _game;		//オーナー

	class CreateAfterImageComponent* _afterImageCom;	//プレイヤーに追加するコンポーネントのアドレスを保持する

	//ライツアウトの制限時間
	int _frameCnt;

	//使用できる回数
	int _useTimes;

	//ライツアウトが使用可能になった時だけしたい処理があるため、それをエナジーの数の変化を比較して、判断する
	int _oldEnergyCount;

	//処理ステップ
	enum class STATE {
		kNone,
		kStart,
		kProcess,
		kEnd
	}_state;
};