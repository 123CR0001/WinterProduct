#pragma once
#include"Component.h"
#include<unordered_map>
#include<string>
#include<functional>

#include"MotionData.h"

class MotionComponent :public Component {
public:
	MotionComponent(class CharaBase* owner, int order = 100);
	~MotionComponent();

	bool Process()override;

	void ResetMotionCount() { _motCnt = 0; }
	void IncrementMotionCount() { _motCnt++; }


	void RegisterCustomCommand(std::string name, std::function<void(const MOTION_DATA_ITEM&)> func) { _customCommandFuncMap[name] = func; }


private:
	class CharaBase* _chara;
	int _motCnt;

	//�S�L�������ʂ̃R�}���h
	static std::unordered_map<std::string, std::function<void(const MOTION_DATA_ITEM&)>>_commandFuncMap;

	//�L�����ŗL�̃R�}���h
	std::unordered_map<std::string, std::function<void(const MOTION_DATA_ITEM&)>>_customCommandFuncMap;
};