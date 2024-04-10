#include"Knife.h"
#include"CharaBase.h"
#include"ObjectServer.h"
#include"CommonSoldier.h"
#include"FrameComponent.h"
#include"Component.h"
#include"PhysWorld.h"
#include"CapsuleComponent.h"
#include"ModeGame.h"
#include"ModeEffekseer.h"
#include"Player.h"
#include"CameraComponent.h"
#include"CharaBase.h"
#include"SoundComponent.h"
#include"ApplicationGlobal.h"

bool Knife::Initialize() {

	WeaponBase::Initialize();

	LoadModel("res/Object/Knife_toDX/knife.mv1", "AttackCollision");
	_attachIndex = MV1SearchFrame(_handle, "AttackCollision");
	MV1SetupCollInfo(_handle, _attachIndex, 8, 8, 8);

	_scale = Vector3(1.f, 1.f, 1.f);
	_eulerAngle = Vector3(2.38f,6.14f,1.79f);
	_pos = Vector3(-12.79f, 1.42f, 8.51f);
	return true;
}

bool Knife::Process() {
	WeaponBase::Process();

	if (_isAttack) {
		PhysWorld::CollisionDetectionResult result = _frame->GetOverlapResult();

		//カプセルコンポーネントとぶつかったか、自分を装備しているキャラじゃないか
		if (result.isHit 
			&& result.item._object != _equippedChara 
			&& result.item._object->GetState() == STATE::kActive
			) {

			CharaBase::DamageData damageData = { true,this,result.item };

			result.item._object->ApplyDamage(damageData);

			//音を生成
			NEW SoundComponent(result.item._object, 300.f);

			gGlobal._sndServer.Get("SE_03")->Play();
		}
	}

	return true;
}
