#pragma once
#include "Component.h"
#include"appframe.h"
#include<unordered_map>
#include<string>
class AIComponent :public Component {
public:
	AIComponent(class ObjectBase* owner,int order = 100);
	virtual ~AIComponent();

	virtual bool Process();

	class AIState* GetCurrentState()const { return _currentState; }

	//登録されているAIStateを返す
	class AIState* GetStateMap(std::string name){
		if (_stateMap.find(name) != _stateMap.end()) {
			return _stateMap[name];
		}
		else { return nullptr; }
	}

	bool ChangeState(const std::string& name);

	void RegisterState(class AIState* state);

	std::vector<Vector3D>&GetPoints(const std::string& name) { return _points[name]; }
	void AddPoint(const std::string& name, const Vector3D& point);
	void DeletePoint(const std::string& name, int num = -1);

protected:
	//AIのクラスを登録
	std::unordered_map<std::string, class AIState*>_stateMap;
	
	//処理をするAIクラス
	class AIState* _currentState;

	//経路の座標を登録
	std::unordered_map<std::string, std::vector<Vector3D>>_points;
};