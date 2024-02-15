
#include "UISelectMap.h"
#include "ModeGame.h"
#include "ModeColorIn.h"
#include"ModeColorOut.h"


UISelectMap::UISelectMap(float initPosX, float initPosY, float endPosX, float endPosY, int frame, std::string stage)
	:UISlide(initPosX, initPosY, endPosX, endPosY, frame) {
	_stageNum = stage;
}

int UISelectMap::Selected() {
	// Ÿ‚Ìƒ‚[ƒh‚ğ“o˜^
	ModeColorIn* colorIn = new ModeColorIn(30, true);

	auto func = [this]() {
		ModeServer::GetInstance()->Del(ModeServer::GetInstance()->Get("select"));
		ModeServer::GetInstance()->Add(new ModeGame(_stageNum), 10, "game");
		};


	ModeBase* mode = new ModeColorOut(colorIn, func, 30);
	ModeServer::GetInstance()->Add(mode, 11, "Out");
	ModeServer::GetInstance()->Get("game");
	return 0;
}
