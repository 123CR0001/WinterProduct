
#include "ApplicationMain.h"
#include "UIContorl.h"
#include "UIServer.h"
#include "ModePause.h"
#include "UIDisplay.h"

UIContorl::UIContorl(float initPosX, float initPosY, float endPosX, float endPosY, int frame)
	:UISlide(initPosX, initPosY, endPosX, endPosY, frame)
{
 }


//void UIContorl::Process() {
//	
//}

void UIContorl::Render() {

	auto ConvertX = [](float posX) {
		float disp = ApplicationMain::GetInstance()->DispSizeW();
		float result = disp * posX / 1920;
		return result;
	};
	auto ConvertY = [](float posY) {
		float disp = ApplicationMain::GetInstance()->DispSizeH();
		float result = disp * posY / 1080;
		return result;
	};

	ModePause* mdPause = static_cast<ModePause*>(_param);
	int num = mdPause->GetSelect();

	if (_selectNum == num) {
		float x = ConvertX(_x);
		float y = ConvertY(_y) - ConvertY(5);
		float w = x + ConvertX(_w) + ConvertX(51);
		float h = ConvertY(_y) + ConvertY(_h) + ConvertY(5);
		DrawExtendGraph(x, y, w, h, _cg, TRUE);
	}
	else {
		float x = ConvertX(_x);
		float y = ConvertY(_y);
		float w = x + ConvertX(_w);
		float h = y + ConvertY(_h);
		DrawExtendGraph(x, y, w, h, _cg, TRUE);
	}
}

int UIContorl::Selected() {
	return 1;
}

