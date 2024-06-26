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
	Vector3 GetPos() const{ return _pos; }
	Vector3 GetEulerAngle() const{ return _eulerAngle; }
	Vector3 GetScale()const { return _scale; }

	//
	void SetPos(const Vector3& pos) { _pos = pos; }
	void SetEulerAngle(const Vector3& set) { _eulerAngle = set; }
	void SetEulerAngleDeg(const Vector3& set) { _eulerAngle = Vector3(DegToRad(set.x), DegToRad(set.y),DegToRad(set.z)); }
	void SetScale(const Vector3& set) { _scale = set; }

	void AddPos(const Vector3& vector) { _pos += vector; }
	void AddEulerAngle(const Vector3& add) { _eulerAngle += add; }
	//XZ平面での前方ベクトル
	Vector3 GetForward()const { return Vector3(sinf(_eulerAngle.y), 0.f, cosf(_eulerAngle.y)); }

	int GetHandle() { return _handle; }
	int GetAttachIndex()const { return _attachIndex;}

	std::string GetName()const { return _name; }

	//jsonデータの格納
	virtual void SetJsonDataUE(nlohmann::json j);

	//回転縮小平行移動を計算し、モデルに適用する
	void ModelMatrixSetUp();

	//オーナーの取得
	class ObjectServer* GetObjectServer()const { return _server; }

	//コンポーネントの追加/削除
	void AddComponent(class Component* component);
	void DeleteComponent(class Component* component);


	//applyDamage
	struct DamageData {
		//ダメージが与えられたか
		bool isDamage = false;
		//誰がダメージを与えたか
		ObjectBase* object = nullptr;
		//アイテム
		PhysWorld::CollisionDetectionItem item;
	};

	//ダメージを与える
	void ApplyDamage(const DamageData& data);

	//ダメージデータの取得
	DamageData GetDamageData()const {return _damageData;}

	enum class STATE {
		kNone,
		kActive,
		kDead
	};

	STATE GetState()const { return _state; }

protected:
	//モデルのハンドル
	int _handle;
	//モデルの当たり判定用フレームのアタッチ番号
	int _attachIndex;

	Vector3 _pos;				//ワールド座標

	Vector3 _eulerAngle;		//角度

	Vector3 _scale;				//拡縮

	std::string _name;			//名前

	DamageData _damageData;		//ダメージデータ

	STATE _state;				//状態

private:
	class ObjectServer* _server;					//オーナー

	std::vector<class Component*>_components;
	std::vector<class Component*>_addComponents;
	std::vector<class Component*>_deleteComponents;

};