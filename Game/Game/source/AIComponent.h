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

	//AIStateを変更する
	bool ChangeState(const std::string& name);

	//AIStateを登録する
	void RegisterState(class AIState* state);

	//経路の座標を返す
	std::vector<Vector3>&GetPoints(const std::string& name) { return _points[name]; }

	//経路の座標を登録
	void AddPoint(const std::string& name, const Vector3& point);

	//経路の座標を挿入
	void InsertPoint(const std::string& name, const Vector3& point, const int num);

	//経路の座標を削除
	void DeletePoint(const std::string& name, int num = -1);

	//視界の設定
	void SetViewAngle(const float angle) { _viewAngle = angle; }
	void SetViewDist(const float dist) { _viewDist = dist; }
	void SetViewLocalPos(const Vector3& viewLocalPos) { _viewLoacalPos = viewLocalPos; }

	//視界の取得
	float GetViewAngle()const { return _viewAngle; }				
	float GetViewDist()const { return _viewDist; }					
	Vector3 GetViewLoaclaPos()const { return _viewLoacalPos; }		

	//オブジェクトが視界に入っているか
	bool IsFound(class ObjectBase* target);

	//オブジェクトが視界に入っているか(_chaseObjectNameから)
	ObjectBase* IsFound();

	//追跡するオブジェクトを設定
	void SetChaseObject(class ObjectBase* target) { _chaseObj = target; }

	//追跡するオブジェクトを取得
	class ObjectBase* GetChaseObject()const { return _chaseObj; }

	//追跡するオブジェクトの名前リストを取得
	std::vector<std::string>& GetChaseObjectName() { return _chaseObjectName; }

	//経路を移動する
	bool MoveTo(std::vector<Vector3>& points, int& num);

protected:
	//AIのクラスを登録
	std::unordered_map<std::string, class AIState*>_stateMap;
	
	//処理をするAIクラス
	class AIState* _currentState;


	//AIStateに必要な情報
	//経路の座標を登録
	std::unordered_map<std::string, std::vector<Vector3>>_points;

	//視界範囲
	float _viewAngle;												//視界角度
	float _viewDist;												//視界距離
	Vector3 _viewLoacalPos;								//視界のローカル位置

	class ObjectBase* _chaseObj;						//追跡するオブジェクト

	std::vector<std::string>_chaseObjectName;	//追跡するオブジェクトの名前
};