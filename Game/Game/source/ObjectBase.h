#pragma once
#include"DxLib.h"
#include"appFrame.h"
#include"../../../include/nlohmann/json.hpp"
class ObjectBase {
public:
	ObjectBase(class ObjectServer* _server);
	virtual ~ObjectBase();

	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();

	virtual bool LoadModel(std::string fileName, std::string attachFrameName = "");

	void AddPos(Vector3D vector) { _pos += vector; }
	VECTOR GetDxPos(){ return DxConverter::VecToDx(_pos); }
	Vector3D GetPos() { return _pos; }
	Vector3D GetOldPos() { return _oldPos; }
	Vector3D GetEulerAngle() { return _eulerAngle; }

	//
	void SetPos(Vector3D pos) { _pos = pos; }
	void SetEulerAngle(Vector3D set) { _eulerAngle = set; }
	void SetEulerAngleDeg(Vector3D set) { _eulerAngle = Vector3D(DegToRad(set.x), DegToRad(set.y),DegToRad(set.z)); }
	void SetScale(Vector3D set) { _scale = set; }

	void AddEulerAngle(Vector3D add) { _eulerAngle += add; }

	int GetHandle()& { return _handle; }
	int GetAttachIndex()const { return _attachIndex;}

	//json�f�[�^�̊i�[
	virtual void SetJsonDataUE(nlohmann::json j);

	//��]�k�����s�ړ����v�Z���A���f���ɓK�p����
	void ModelMatrixSetUp();

	//
	class ObjectServer* GetObjectServer()const { return _server; }

	//
	void AddComponent(class Component* component);
	void DeleteComponent(class Component* component);

protected:
	//���f���̃n���h��
	int _handle;
	//���f���̓����蔻��p�t���[���̃A�^�b�`�ԍ�
	int _attachIndex;

	//�ʒu���
	Vector3D _pos;
	Vector3D _oldPos;

	//�p�x
	Vector3D _eulerAngle;

	//�g�k
	Vector3D _scale;

	//
	MATRIX _matrix;

	//�����蔻��̃R���|�[�l���g
	class FrameComponent* _frame;
private:
	class ObjectServer* _server;

	std::vector<class Component*>_components;
	std::vector<class Component*>_addComponents;
	std::vector<class Component*>_deleteComponents;
};