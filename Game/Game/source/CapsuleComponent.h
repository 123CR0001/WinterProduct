#pragma once
#include"Component.h"
#include"PhysWorld.h"

class CapsuleComponent :public Component {
public:
	CapsuleComponent(ObjectBase* owner, int order = 100);
	virtual ~CapsuleComponent();

	void SetMember(float length, float radius, Vector3 diff = Vector3(0.f, 0.f, 0.f)) { _length = length; _radius = radius; }

	void AddSkipName(const char* name) { _skipNames.emplace_back(name); }

	Capsule GetCapsule()const;

	bool Process()override;

	PhysWorld::CollisionDetectionResult GetOverlapResult();
	PhysWorld::CollisionDetectionResult GetEventOverlapResult()const { return _result; };

	//重なり発生時
	class ObjectBase* GetBeginOverlap() const{
		if(_oldResult.item._object != _result.item._object) {
			return _result.item._object;
		}

		return nullptr;
	}

	//重なり終了時
	class ObjectBase* GetEndOverlap() const{
		if(_oldResult.item._object != _result.item._object) {
			return _oldResult.item._object;
		}
		return nullptr;
	}
protected:
	float _length;										//カプセルの長さ
	float _radius;										//カプセルの半径
	Vector3 _diff;										//オブジェクトの位置からどれだけ離れているか

	PhysWorld::CollisionDetectionResult _result;		//衝突判定結果
	PhysWorld::CollisionDetectionResult _oldResult;		//前回の衝突判定結果

	//押し出し処理をしないキャラの名前を登録
	std::vector<std::string>_skipNames;
};