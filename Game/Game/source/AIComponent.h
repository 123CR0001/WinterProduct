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
	void InsertPoint(const std::string& name, const Vector3D& point, const int num);
	void DeletePoint(const std::string& name, int num = -1);

	void SetViewAngle(const float angle) { _viewAngle = angle; }
	void SetViewDist(const float dist) { _viewDist = dist; }
	void SetView(const Vector3D& view) { _view = view; }

	float GetViewAngle()const { return _viewAngle; }
	float GetViewDist()const { return _viewDist; }
	Vector3D GetView()const { return _view; }

	bool IsFound(ObjectBase* target);
	bool MoveTo(std::vector<Vector3D>& points, int& num);

protected:
	//AIのクラスを登録
	std::unordered_map<std::string, class AIState*>_stateMap;
	
	//処理をするAIクラス
	class AIState* _currentState;

	//経路の座標を登録
	std::unordered_map<std::string, std::vector<Vector3D>>_points;

	//視界範囲
	float _viewAngle;
	float _viewDist;
	Vector3D _view;
};