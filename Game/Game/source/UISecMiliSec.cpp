#include"UISecMiliSec.h"
#include"SpriteNumber.h"

UISecMiliSec::UISecMiliSec(const Transform2& transform)
	:UI()
	, _sec(std::make_unique<SpriteNumber>(1, 2))
	, _miliSec(std::make_unique<SpriteNumber>(1, 2))
	, _colon(std::make_unique<SpriteText>(LoadGraph("res/UI/Result/ui_timer_02.png"), transform,Vector2(22.5f,105.f)))
	, _pos(transform.pos)
	, _isDraw(true)
	,_frameCnt(0)
{
	_sec->LoadDivNumber("res/UI/Result/ui_timer_01.png", 5, 2, 46, 70);
	_sec->SetSize(Vector2(69, 105));
	_miliSec->LoadDivNumber("res/UI/Result/ui_timer_01.png", 5, 2, 46, 70);
	_miliSec->SetSize(Vector2(69, 105));
}

UISecMiliSec::~UISecMiliSec(){}

bool UISecMiliSec::Process() {

	//描画する値の更新

	int sec = _frameCnt / 60;

	//2桁までしか描画しないため、繰り上げは2桁しか行わない
	float miliSec = static_cast<float>(_frameCnt) / 60.f - static_cast<float>(sec);

	miliSec *= 100.f;

	_sec->SetNumber(sec);
	_miliSec->SetNumber(static_cast<int>(miliSec));

	return true;
}

bool UISecMiliSec::Draw() {

	if (!_isDraw)return true;

	Vector2 secPos(_pos.x - _colon->GetSize().x / 2.f - _sec->GetSize().x / 2.f, _pos.y);
	Vector2 miliSecPos(_pos.x + _colon->GetSize().x / 2.f + _sec->GetSize().x / 2.f * 3.f, _pos.y);

	_sec->SetPos(secPos);
	_miliSec->SetPos(miliSecPos);

	_sec->Draw();
	_miliSec->Draw();
	_colon->Draw();


	return true;
}
