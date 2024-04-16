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

	//AIState��ύX����
	bool ChangeState(const std::string& name);

	//AIState��o�^����
	void RegisterState(class AIState* state);

	//�o�H�̍��W��Ԃ�
	std::vector<Vector3>&GetPoints(const std::string& name) { return _points[name]; }

	//�o�H�̍��W��o�^
	void AddPoint(const std::string& name, const Vector3& point);

	//�o�H�̍��W��}��
	void InsertPoint(const std::string& name, const Vector3& point, const int num);

	//�o�H�̍��W���폜
	void DeletePoint(const std::string& name, int num = -1);

	//���E�̐ݒ�
	void SetViewAngle(const float angle) { _viewAngle = angle; }
	void SetViewDist(const float dist) { _viewDist = dist; }
	void SetViewLocalPos(const Vector3& viewLocalPos) { _viewLoacalPos = viewLocalPos; }

	//���E�̎擾
	float GetViewAngle()const { return _viewAngle; }				
	float GetViewDist()const { return _viewDist; }					
	Vector3 GetViewLoaclaPos()const { return _viewLoacalPos; }		

	//�I�u�W�F�N�g�����E�ɓ����Ă��邩
	bool IsFound(class ObjectBase* target);

	//�I�u�W�F�N�g�����E�ɓ����Ă��邩(_chaseObjectName����)
	ObjectBase* IsFound();

	//�ǐՂ���I�u�W�F�N�g��ݒ�
	void SetChaseObject(class ObjectBase* target) { _chaseObj = target; }

	//�ǐՂ���I�u�W�F�N�g���擾
	class ObjectBase* GetChaseObject()const { return _chaseObj; }

	//�ǐՂ���I�u�W�F�N�g�̖��O���X�g���擾
	std::vector<std::string>& GetChaseObjectName() { return _chaseObjectName; }

	//�o�H���ړ�����
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
	float _viewAngle;												//���E�p�x
	float _viewDist;												//���E����
	Vector3 _viewLoacalPos;								//���E�̃��[�J���ʒu

	class ObjectBase* _chaseObj;						//�ǐՂ���I�u�W�F�N�g

	std::vector<std::string>_chaseObjectName;	//�ǐՂ���I�u�W�F�N�g�̖��O
};