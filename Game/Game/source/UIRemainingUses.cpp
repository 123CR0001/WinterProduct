
#include "UIRemainingUses.h"

#include "ApplicationMain.h"
#include "ModeGame.h"

//UIRemainingUses::UIRemainingUses(ModeGame* game, float initPosX, float initPosY, float endPosX, float endPosY, int frame)
//	:UISlide(initPosX, initPosY, endPosX, endPosY, frame){
//	_game = game;
//
//	int cgRemain[REMAIN_NUM];
//	_mCg["bg"].push_back(res::LoadGraph("res/UI/Game/Item/ui_itembg_01.png"));
//	res::LoadDivGraph("res/UI/Game/Timer/ui_timer_01.png", 10, 5, 2, 46, 70, cgRemain);
//
//	for (int i = 0; i < REMAIN_NUM; i++) {
//		_mCg["number"].push_back(cgRemain[i]);
//	}
//}
//
//void UIRemainingUses::Terminate() {
//	_mCg.clear();
//}
//
//void UIRemainingUses::Render() {
//	auto ConvertX = [](float posX) {
//		float disp = ApplicationMain::GetInstance()->DispSizeW();
//		float result = disp * posX / 1920;
//		return result;
//		};
//	auto ConvertY = [](float posY) {
//		float disp = ApplicationMain::GetInstance()->DispSizeH();
//		float result = disp * posY / 1080;
//		return result;
//		};
//	int x = ConvertX(_x);
//	int y = ConvertY(_y);
//	int w = ConvertX(_x) + ConvertX(_w);
//	int h = ConvertY(_y) + ConvertY(_h);
//	// ”wŒi
//	DrawExtendGraph(x, y, w, h, _mCg["bg"][0], TRUE);
//	x = x + ConvertX(270);
//	y = y + ConvertY(40);
//	w = x + ConvertX(46);
//	h = y + ConvertY(70);
//	// ”Žš
//	DrawExtendGraph(x, y, w, h, _mCg["number"][_game->GetCnt() / 10], TRUE);
//	x = x + ConvertX(50);
//	//y = y + ConvertY(40);
//	w = x + ConvertX(46);
//	h = y + ConvertY(70);
//	// ”Žš
//	DrawExtendGraph(x, y, w, h, _mCg["number"][_game->GetCnt() % 10], TRUE);
//}
//
//int UIRemainingUses::Selected() {
//	SetSlideUIPosition(_x, _y, -500, _initPosY, _frame);
//	return 0;
//}
