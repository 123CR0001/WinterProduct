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

	void SetJsonDataUE(nlohmann::json data)override;

	float GetDetectionLevel()const { return _detectionLevel; }

	class AIComponent* GetAIComponent()const { return _AI; }

protected:

	float _detectionLevel;	//ŒŸ’m“x

	class AIComponent* _AI;
	class MoveComponent* _moveCom;
	class CapsuleComponent* _capsule;

};