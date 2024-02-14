
#include "AppFrame.h"
#include "ApplicationGlobal.h"
#include<fstream>
#include"../../../include/nlohmann/json.hpp"


// ŽÀ‘Ì
ApplicationGlobal		gGlobal;

ApplicationGlobal::ApplicationGlobal() {

}

ApplicationGlobal::~ApplicationGlobal() {
}


bool ApplicationGlobal::Init() {
	LoadMotionData();
	return true;
}

#include"Player.h"

void ApplicationGlobal::LoadMotionData() {
	MotionDataJson data("res/MotionData/test.json");
	_charaMotionData[MotionType::PLAYER]["Attack"] = data.GetMotionData();

	data = MotionDataJson("res/MotionData/test2.json");
	_charaMotionData[MotionType::PLAYER]["Attack2"] = data.GetMotionData();
}