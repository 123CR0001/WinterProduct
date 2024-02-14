#include"Decoy.h"
#include"Player.h"
#include"ObjectServer.h"
#include"CapsuleComponent.h"
#include"AnimationComponent.h"

Decoy::Decoy(Player* player)
	:ObjectBase(player->GetObjectServer(),false,"Decoy")
	,_capsule(NEW CapsuleComponent(this,1000))
	,_anim(NEW AnimationComponent(this))
	,_hitCnt(1)
{
	//�v���C���[�̑O���ɔz�u
	_pos = player->GetPos() + player->GetForward() * 70.f;
	_eulerAngle = player->GetEulerAngle();

	_capsule->SetMember(100.f, 40.f);
}

Decoy::~Decoy() {

}

bool Decoy::Initialize() {
	ObjectBase::Initialize();

	LoadModel("res/cg_player_girl/cg_player_girl.mv1");

	_anim->LoadAnimation("Run", "cg_player_run", 0);

	_anim->ChangeAnimation("Run");

	return true;
}

bool Decoy::Terminate() {
	ObjectBase::Terminate();
	return true;
}

bool Decoy::Process() {
	ObjectBase::Process();

	PhysWorld::CollisionDetectionResult event = _capsule->GetEventOverlapResult();

	//�Ȃɂ��ƂԂ�������A���˂���
	if (event.isHit) {
		Vector3D ref = Vector3D::Reflect(GetForward(), event.item.normal);
		_eulerAngle.y = atan2f(ref.x, ref.z);

		//0�ɂȂ�����A�������폜
		if (_hitCnt <= 0) {
			GetObjectServer()->DeleteObject(this);
		}

		//�Ԃ�������A�f�N�������g
		_hitCnt--;
	}

	AddPos(GetForward() * 2.0f);

	return true;
}