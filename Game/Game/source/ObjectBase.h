#pragma once
#include"DxLib.h"
#include"appFrame.h"
#include"../../../include/nlohmann/json.hpp"
#include"PhysWorld.h"

class ObjectBase {
public:
	ObjectBase(class ObjectServer* _server, bool isFrane = false, std::string name = "");
	virtual ~ObjectBase();

	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();

	virtual bool LoadModel(std::string fileName, std::string attachFrameName = "");

	VECTOR GetDxPos()const{ return DxConverter::VecToDx(_pos); }
	Vector3D GetPos() const{ return _pos; }
	Vector3D GetEulerAngle() const{ return _eulerAngle; }
	Vector3D GetScale()const { return _scale; }

	//
	void SetPos(Vector3D pos) { _pos = pos; }
	void SetEulerAngle(Vector3D set) { _eulerAngle = set; }
	void SetEulerAngleDeg(Vector3D set) { _eulerAngle = Vector3D(DegToRad(set.x), DegToRad(set.y),DegToRad(set.z)); }
	void SetScale(Vector3D set) { _scale = set; }

	void AddPos(Vector3D vector) { _pos += vector; }
	void AddEulerAngle(Vector3D add) { _eulerAngle += add; }
	//XZ���ʂł̑O���x�N�g��
	Vector3D GetForward()const { return Vector3D(sinf(_eulerAngle.y), 0.f, cosf(_eulerAngle.y)); }

	int GetHandle()& { return _handle; }
	int GetAttachIndex()const { return _attachIndex;}

	std::string GetName()const { return _name; }

	//json�f�[�^�̊i�[
	virtual void SetJsonDataUE(nlohmann::json j);

	//��]�k�����s�ړ����v�Z���A���f���ɓK�p����
	void ModelMatrixSetUp();

	//
	class ObjectServer* GetObjectServer()const { return _server; }

	//�R���|�[�l���g�̒ǉ�/�폜
	void AddComponent(class Component* component);
	void DeleteComponent(class Component* component);


	//applyDamage
	struct DamageData {
		//�_���[�W���^����ꂽ��
		bool isDamage = false;
		//�N���_���[�W��^������
		ObjectBase* object = nullptr;

		PhysWorld::CollisionDetectionItem item;

	};
	void ApplyDamage(const DamageData& data);

protected:
	//���f���̃n���h��
	int _handle;
	//���f���̓����蔻��p�t���[���̃A�^�b�`�ԍ�
	int _attachIndex;

	//�ʒu���
	Vector3D _pos;

	//�p�x
	Vector3D _eulerAngle;

	//�g�k
	Vector3D _scale;

	//
	MATRIX _matrix;

	const std::string _name;

	DamageData _damageData;

private:
	class ObjectServer* _server;

	std::vector<class Component*>_components;
	std::vector<class Component*>_addComponents;
	std::vector<class Component*>_deleteComponents;

};