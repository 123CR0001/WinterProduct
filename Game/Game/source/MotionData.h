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

//1���[�V�����ɓo�^�����f�[�^
typedef std::vector<MOTION_DATA_ITEM> MOTION_DATA_ITEMS;

//�S�Ẵ��[�V�����f�[�^
typedef std::unordered_map<std::string/*���[�V������*/, MOTION_DATA_ITEMS> MOTION_DATA;

//���[�V�����f�[�^�̓ǂݍ��ݗp�N���X(.Json)
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
