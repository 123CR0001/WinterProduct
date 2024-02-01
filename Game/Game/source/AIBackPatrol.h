#pragma once
#include"AIState.h"
#include"AppFrame.h"

class AIBackPatrol :public AIState {
public:
	AIBackPatrol(class AIComponent* owner);
	~AIBackPatrol();

	bool Process()override;

	void OnEnter()override;
	void OnExist()override;

	const char* GetName()override { return "BackPatrol"; }

	//void SetData(void* data);

	std::vector<Vector3D> GetPoints()const { return _points; }
	int GetPointsNum()const { return _pointsNum; }
	void AddBackPatrol(Vector3D add) { _points.insert(_points.begin(), add); }
	void ClearBackPatrol() {
		_points.clear(); 	//ƒ‹[ƒg‚Ì‰Šú‰»
		_pointsNum = 0;
	}
private:
	std::vector<Vector3D>_points;
	int _pointsNum;

};
