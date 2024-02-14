#pragma once
#include"DxLib.h"
#include"appFrame.h"
#include"../../../include/nlohmann/json.hpp"
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

	int GetHandle()& { return _handle; }
	int GetAttachIndex()const { return _attachIndex;}

	std::string GetName()const { return _name; }

	//jsonデータの格納
	virtual void SetJsonDataUE(nlohmann::json j);

	//回転縮小平行移動を計算し、モデルに適用する
	void ModelMatrixSetUp();

	//
	class ObjectServer* GetObjectServer()const { return _server; }

	//
	void AddComponent(class Component* component);
	void DeleteComponent(class Component* component);

protected:
	//モデルのハンドル
	int _handle;
	//モデルの当たり判定用フレームのアタッチ番号
	int _attachIndex;

	//位置情報
	Vector3D _pos;

	//角度
	Vector3D _eulerAngle;

	//拡縮
	Vector3D _scale;

	//
	MATRIX _matrix;

	const std::string _name;

private:
	class ObjectServer* _server;

	std::vector<class Component*>_components;
	std::vector<class Component*>_addComponents;
	std::vector<class Component*>_deleteComponents;
};