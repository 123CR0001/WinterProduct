
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

void ApplicationGlobal::LoadMotionData() {
	//player
	MotionDataJson data("res/MotionData/Player/Attack1.json");
	if (data.IsSuccess()) {
		_charaMotionData["player"]["Attack"] = data.GetMotionData();
	}

	data = MotionDataJson("res/MotionData/Player/Attack2.json");
	if (data.IsSuccess()) {
		_charaMotionData["player"]["Attack2"] = data.GetMotionData();
	}

	data = MotionDataJson("res/MotionData/Player/Attack3.json");
	if (data.IsSuccess()) {
		_charaMotionData["player"]["Attack3"] = data.GetMotionData();
	}

	data = MotionDataJson("res/MotionData/Player/Walk.json");
	if (data.IsSuccess()) {
		_charaMotionData["player"]["run"] = data.GetMotionData();
	}

	data = MotionDataJson("res/MotionData/Player/Dead.json");
	if (data.IsSuccess()) {
		_charaMotionData["player"]["Dead"] = data.GetMotionData();
	}
	data = MotionDataJson("res/MotionData/Player/Clear.json");
	if (data.IsSuccess()) {
		_charaMotionData["player"]["Clear"] = data.GetMotionData();
	}



	//CommonSoldier
	data = MotionDataJson("res/MotionData/CommonSoldier/Discovery.json");
	if (data.IsSuccess()) {
		_charaMotionData["CommonSoldier"]["Discovery"] = data.GetMotionData();
	}

	data = MotionDataJson("res/MotionData/CommonSoldier/Losesight.json");
	if (data.IsSuccess()) {
		_charaMotionData["CommonSoldier"]["LoseSight"] = data.GetMotionData();
	}

	data = MotionDataJson("res/MotionData/CommonSoldier/Shoot.json");
	if (data.IsSuccess()) {
		_charaMotionData["CommonSoldier"]["Shoot"] = data.GetMotionData();
	}

	data = MotionDataJson("res/MotionData/CommonSoldier/Walk.json");
	if (data.IsSuccess()) {
		_charaMotionData["CommonSoldier"]["Walk"] = data.GetMotionData();
	}

	data = MotionDataJson("res/MotionData/CommonSoldier/Dead.json");
	if (data.IsSuccess()) {
		_charaMotionData["CommonSoldier"]["Death"] = data.GetMotionData();
	}

	data = MotionDataJson("res/MotionData/CommonSoldier/BlindWalk.json");
	if(data.IsSuccess()) {
		_charaMotionData["CommonSoldier"]["BlindWalk"] = data.GetMotionData();
	}

	data = MotionDataJson("res/MotionData/CommonSoldier/Panic.json");
	if(data.IsSuccess()) {
		_charaMotionData["CommonSoldier"]["Panic"] = data.GetMotionData();
	}

	data = MotionDataJson("res/MotionData/CommonSoldier/LookAround.json");
	if(data.IsSuccess()) {
		_charaMotionData["CommonSoldier"]["LookAround"] = data.GetMotionData();
	}
}