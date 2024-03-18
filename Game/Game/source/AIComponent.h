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

	//�o�^����Ă���AIState��Ԃ�
	class AIState* GetStateMap(std::string name){
		if (_stateMap.find(name) != _stateMap.end()) {
			return _stateMap[name];
		}
		else { return nullptr; }
	}

	bool ChangeState(const std::string& name);

	void RegisterState(class AIState* state);

	std::vector<Vector3>&GetPoints(const std::string& name) { return _points[name]; }
	void AddPoint(const std::string& name, const Vector3& point);
	void InsertPoint(const std::string& name, const Vector3& point, const int num);
	void DeletePoint(const std::string& name, int num = -1);

	void SetViewAngle(const float angle) { _viewAngle = angle; }
	void SetViewDist(const float dist) { _viewDist = dist; }
	void SetView(const Vector3& view) { _view = view; }

	float GetViewAngle()const { return _viewAngle; }
	float GetViewDist()const { return _viewDist; }
	Vector3 GetView()const { return _view; }

	bool IsFound(class ObjectBase* target);
	void SetChaseObject(class ObjectBase* target) { _chaseObj = target; }
	class ObjectBase* GetChaseObject()const { return _chaseObj; }

	bool MoveTo(std::vector<Vector3>& points, int& num);

protected:
	//AI�̃N���X��o�^
	std::unordered_map<std::string, class AIState*>_stateMap;
	
	//����������AI�N���X
	class AIState* _currentState;


	//AIState�ɕK�v�ȏ��
	//�o�H�̍��W��o�^
	std::unordered_map<std::string, std::vector<Vector3>>_points;

	//���E�͈�
	float _viewAngle;
	float _viewDist;
	Vector3 _view;

	//���F�����L����
	class ObjectBase* _chaseObj;
};