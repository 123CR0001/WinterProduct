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

	//�v���C���[�����E�͈͓��ɂ��邩
	bool IsPlayerFound();

	void SetJsonDataUE(nlohmann::json data)override;

	//�ڕW�n�_�ɏ��X�Ɉړ�
	virtual bool MoveRoute(std::vector<Vector3D>& points, int& num);

	float GetVisionDist()const { return _visionDist; }
	float GetVisionAngle()const { return _visionAngle; }

	float GetDetectionLevel()const { return _detectionLevel; }

	class AIComponent* GetAIComponent()const { return _AI; }
	class MoveComponent* GetMoveComponent()const { return _moveCom; }

protected:
	float _visionDist;		//����͈̔�(����)
	float _visionAngle;		//����͈̔�(�p�x)

	float _detectionLevel;	//���m�x

	class AIComponent* _AI;
	class MoveComponent* _moveCom;

};