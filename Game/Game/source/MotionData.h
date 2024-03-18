#pragma once
#include"../../../AppFrame/source/Math/Json.h"
#include<string>
#include<vector>
#include<unordered_map>
#include"Player.h"

struct MOTION_DATA_ITEM {
	int playTime = 0;
	std::string command;
	float vector = 0.f;
	float vectorScale = 0.f;
	int attackPoint = 0;
	std::string effectPlayName;
	std::string effectPlay3DFrame;
	std::string soundPlayName;
	std::string ChangeMotion;
};

//1モーションに登録されるデータ
typedef std::vector<MOTION_DATA_ITEM> MOTION_DATA_ITEMS;

//全てのモーションデータ
typedef std::unordered_map<std::string/*モーション名*/, MOTION_DATA_ITEMS> MOTION_DATA;

//モーションデータの読み込み用クラス(.Json)
class MotionDataJson :public Json {
public:
	MotionDataJson(std::string fileName) {
		if (LoadData(fileName)) {
			_success = true;
			return;
		}
		_success = false;
	}
	virtual ~MotionDataJson() {}

	bool LoadData(std::string file_name)override {
		if (!Json::LoadData(file_name)) { return false; }

		for (auto& data : _jsonData) {
			MOTION_DATA_ITEM item = {
				data.at("PlayTime"),
				data.at("Command"),
				data.at("Vector"),
				data.at("VectorScale"),
				data.at("AttackPoint"),
				data.at("Effect"),
				data.at("Play3DFrame"),
				data.at("Sound"),
				data.at("ChangeMotion")
			};
			_motionData.emplace_back(item);
		}

		return true;
	}

	MOTION_DATA_ITEMS GetMotionData()const { return _motionData; }

	MOTION_DATA_ITEMS _motionData;

};
