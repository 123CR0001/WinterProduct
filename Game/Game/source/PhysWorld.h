#pragma once
#include"PhysWorld.h"
#include"appframe.h"

//ゲーム内での物理世界の処理

class PhysWorld {
public:
	PhysWorld(class ObjectServer* server);
	~PhysWorld();

	//当たり判定結果データ
	struct CollisionDetectionItem {
		Vector3D pushVec;		//
		float pushDistanceSqaure = 0.f;			//押し出す長さ
		class ObjectBase* _object = nullptr;	//重なったオブジェクトのアドレス
		Vector3D hitPosition;
	};
	
	struct CollisionDetectionResult {
		bool isHit = false;
		CollisionDetectionItem item;
	};

	bool IsHear(class ObjectBase* object,Vector3D& pos);

	std::vector<class CapsuleComponent*>& GetCapsuleComponent() { return _capsules; }
	std::vector<class FrameComponent*>& GetFrameComponent() { return _frames; }
	std::vector<class SoundComponent*>& GetSoundComponent() { return _sounds; }
protected:
	class ObjectServer* _server;
	std::vector<class CapsuleComponent*>_capsules;	//カプセル
	std::vector<class FrameComponent*>_frames;			//モデルのフレーム
	std::vector<class SoundComponent*>_sounds;			//ゲーム世界での音(判定は球)
};