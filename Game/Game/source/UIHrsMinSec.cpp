#include"UIHrsMinSec.h"
#include"SpriteNumber.h"
#include"appframe.h"
#include"ApplicationMain.h"

//コロンの描画サイズ
constexpr float COLON_WIDTH = 10.f;
constexpr float COLON_HEIGHT = 35.f;

//数字の描画サイズ
constexpr float NUMBER_WIDTH = 23.f;
constexpr float NUMBER_HEIGHT = 35.f;

constexpr int NUMBER_DIGIT = 2;

UIHrsMinSec::UIHrsMinSec(int frame, int secondTime,const Transform2& transform ,const Vector2& size)
	:_spriteColon(NEW SpriteText())
	, _secondTime(secondTime)
	, _frame(frame)
	, _frameCnt(0)
	, _isDraw(true)
{
	//画面が最大サイズの時の描画サイズ * 上記の比率 = 画面の大きさに適した描画サイズにする

	_spriteColon->SetHandle(ResourceServer::LoadGraph("res/UI/Result/ui_timer_02.png"));
	_spriteColon->SetSize(Vector2(COLON_WIDTH, COLON_HEIGHT));

	for (int a = 0; a < _spriteTimes.size(); a++) {

		_spriteTimes[a] = NEW SpriteNumber(0,NUMBER_DIGIT);
		//画像のハンドル
		_spriteTimes[a]->LoadDivNumber("res/UI/Result/ui_timer_01.png", 5, 2, 46, 70);
		//描画サイズ
		_spriteTimes[a]->SetSize(Vector2(size.x, size.y));
		//描画位置
		_spriteTimes[a]->SetPos(								//時間と分、秒は２桁ずつ描画する	//コロンは一回だけ
			Vector2(transform.pos.x, transform.pos.y) 
			- Vector2(size.x  * static_cast<float>(NUMBER_DIGIT), 0.f) * static_cast<float>(a) 
			- Vector2(COLON_WIDTH , 0.f) * static_cast<float>(a)
		);
	}

}

UIHrsMinSec::~UIHrsMinSec() {

	for (int a = 0; a < _spriteTimes.size(); a++) {
		delete _spriteTimes[a];
	}

	delete _spriteColon;
}

bool UIHrsMinSec::Process() {

	float rate = static_cast<float>(_frameCnt) / static_cast<float>(_frame);
	
	int secondTime = static_cast<int>(EasingLinear(0.f,static_cast<float>(_secondTime), rate));

	//時間の算出
	int hrs = secondTime / 60 / 60;

	//分の算出
	int min = (secondTime - hrs * 60 * 60) / 60;
	
	//秒の算出
	int sec = secondTime - (hrs * 60 * 60) - (min * 60);

	int nums[3] = { sec,min,hrs };

	//0:秒　1:分 2:時間
	for (int a = 0; a < 3; a++) {
		_spriteTimes[a]->SetNumber(nums[a]);
	}

	if (_frameCnt < _frame) {
		_frameCnt++;
	}
	
	return true;
}

bool UIHrsMinSec::Draw() {

	//描画フラグがfalseなら描画しない
	if (!_isDraw)return true;

	//画面の最大横幅1920と現在の画面横幅との比率
	float rateW = static_cast<float>(SCREEN_WIDTH) / static_cast<float>(MAX_SCREEN_WIDTH);

	for (int a = 0; a < _spriteTimes.size(); a++) {
		_spriteTimes[a]->Draw();

		//コロンは時間を描画してからは描画しない
		if (a < _spriteTimes.size() - 1) {			
			//中心座標から描画しているので、/2
			float size = _spriteTimes[a]->GetSize().x;
			_spriteColon->SetPos(_spriteTimes[a]->GetPos() - Vector2(size * static_cast<float>(NUMBER_DIGIT)-size/2.f, 0.f) - Vector2(COLON_WIDTH/2.f, 0.f));
			_spriteColon->Draw();
		}
	}
	return true;
}