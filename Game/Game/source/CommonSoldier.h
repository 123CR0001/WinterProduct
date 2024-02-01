#pragma once
#include"CharaBase.h"



class CommonSoldier : public CharaBase{
public:
	CommonSoldier(class ObjectServer* server);
	virtual ~CommonSoldier();

	bool Initialize()override;
	bool Terminate()override;
	bool Process()override;
	bool Render()override;

	//プレイヤーが視界範囲内にいるか
	bool IsPlayerFound();

	void SetJsonDataUE(nlohmann::json data)override;

	//目標地点に順々に移動
	virtual bool MoveRoute(std::vector<Vector3D>& points, int& num);

	float GetVisionDist()const { return _visionDist; }
	float GetVisionAngle()const { return _visionAngle; }

	class AIComponent* GetAIComponent()const { return _AI; }
	class MoveComponent* GetMoveComponent()const { return _moveCom; }

protected:
	float _visionDist;		//視野の範囲(距離)
	float _visionAngle;		//視野の範囲(角度)

	float _detectionDegree;	//検知度

	class AIComponent* _AI;
	class MoveComponent* _moveCom;

};