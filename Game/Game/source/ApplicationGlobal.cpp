
#include "AppFrame.h"
#include "ApplicationGlobal.h"
#include<fstream>
#include"../../../include/nlohmann/json.hpp"


// é¿ëÃ
ApplicationGlobal		gGlobal;

ApplicationGlobal::ApplicationGlobal() {

}

ApplicationGlobal::~ApplicationGlobal() {
}


bool ApplicationGlobal::Init() {
	LoadMotionData();
	// îÒìØä˙ì«Ç›çûÇ›ê›íË
	SetUseASyncLoadFlag(TRUE);
	ResourceServer::MV1LoadModel("res/Chara/Tracer/tracer_5.mv1");
	ResourceServer::MV1LoadModel("res/Chara/mannequin.mv1");
	ResourceServer::MV1LoadModel("res/Object/siren/siren.mv1");
	ResourceServer::MV1LoadModel("res/cg_player_girl/cg_player_girl.mv1");
	ResourceServer::MV1LoadModel("res/Chara/Soldier_fix/soldier_2_4_IK_A_05.mv1");
	ResourceServer::MV1LoadModel("res/Chara/Owl_toDX/Owl.mv1");
	ResourceServer::MV1LoadModel("res/Object/Gun/Gun.mv1");
	ResourceServer::MV1LoadModel("res/Object/Knife_toDX/knife.mv1");
	// ìØä˙ì«Ç›çûÇ›ê›íË
	SetUseASyncLoadFlag(FALSE);
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
		_charaMotionData["CommonSoldier"]["Dead"] = data.GetMotionData();
	}
}