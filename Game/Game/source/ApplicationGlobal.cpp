
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
	ResourceServer::MV1LoadModel("res/Chara/Tracer/tracer_5.mv1");
	ResourceServer::MV1LoadModel("res/Chara/mannequin.mv1");
	ResourceServer::MV1LoadModel("res/Object/siren/siren.mv1");
	ResourceServer::MV1LoadModel("res/cg_player_girl/cg_player_girl.mv1");
	ResourceServer::MV1LoadModel("res/Chara/soldier_2_4_IK_A.mv1");

	return true;
}

void ApplicationGlobal::LoadMotionData() {
	MotionDataJson data("res/MotionData/PlayerAttack0.json");
	_charaMotionData[MotionType::PLAYER]["Attack"] = data.GetMotionData();

	data = MotionDataJson("res/MotionData/PlayerAttack1.json");
	_charaMotionData[MotionType::PLAYER]["Attack2"] = data.GetMotionData();

	data = MotionDataJson("res/MotionData/PlayerAttack2.json");
	_charaMotionData[MotionType::PLAYER]["Attack3"] = data.GetMotionData();

	data = MotionDataJson("res/MotionData/mo_move_01.json");
	_charaMotionData[MotionType::PLAYER]["run"] = data.GetMotionData();

	data = MotionDataJson("res/MotionData/enemydiscovery.json");
	_charaMotionData["CommonSoldier"]["Discovery"] = data.GetMotionData();

	data = MotionDataJson("res/MotionData/enemylosesight.json");
	_charaMotionData["CommonSoldier"]["LookAround"] = data.GetMotionData();
}