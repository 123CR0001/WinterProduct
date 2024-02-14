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

//モーションデータ用キャラクターの種類
namespace MotionType {
	constexpr auto PLAYER = "player";
	constexpr auto ENEMY = "enemy";
}

//モーションデータ用実行コマンド
namespace MotionCommand {

	constexpr unsigned int ATTACK_ON =				0;
	constexpr unsigned int ATTACK_OFF =				1;
	constexpr unsigned int PLAY_EFFECT =			2;
	constexpr unsigned int PLAY_EFFECT_3DFRAME =	3;
	constexpr unsigned int PLAY_SOUND =				4;
	constexpr unsigned int LOOP =					5;
	constexpr unsigned int CHANGE_MOTION =			6;
	constexpr unsigned int MOVE =					7;

}

//モーションデータの読み込み用クラス(.Json)
class MotionDataJson :public Json {
public:
	MotionDataJson(std::string fileName) {
		if (LoadData(fileName)) {
			_success = true;
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

private:
	std::unordered_map<std::string, unsigned int>ConvertCommand{
		{"ATTACK_ON",MotionCommand::ATTACK_ON},
		{"ATTACK_OFF",MotionCommand::ATTACK_OFF},
		{"PLAY_EFFECT",MotionCommand::PLAY_EFFECT},
		{"PLAY_EFFECT_3DFRAME",MotionCommand::PLAY_EFFECT_3DFRAME},
		{"PLAY_SOUND",MotionCommand::PLAY_SOUND},
		{"LOOP",MotionCommand::LOOP},
		{"CHANGE_MOTION",MotionCommand::CHANGE_MOTION},
		{"MOVE",MotionCommand::MOVE}
	};
};
