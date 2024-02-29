
#include "AppFrame.h"
#include "ApplicationGlobal.h"
#include<fstream>
#include"../../../include/nlohmann/json.hpp"


// ����
ApplicationGlobal		gGlobal;

ApplicationGlobal::ApplicationGlobal() {

}

ApplicationGlobal::~ApplicationGlobal() {
}


bool ApplicationGlobal::Init() {
	LoadMotionData();
	ResourceServer::MV1LoadModel("res/Chara/Tracer/tracer_5.mv1");
	ResourceServer::MV1LoadModel("res/Chara/mannequin.mv1");
	ResourceServer::MV1LoadModel("res/Object/siren/siren.mv1");
	ResourceServer::MV1LoadModel("res/cg_player_girl/cg_player_girl.mv1");
	ResourceServer::MV1LoadModel("res/Chara/soldier_2_4_IK_A.mv1");

	return true;
}

#include"Player.h"

void ApplicationGlobal::LoadMotionData() {
	MotionDataJson data("res/MotionData/test.json");
	_charaMotionData[MotionType::PLAYER]["Attack"] = data.GetMotionData();

	data = MotionDataJson("res/MotionData/test2.json");
	_charaMotionData[MotionType::PLAYER]["Attack2"] = data.GetMotionData();
}