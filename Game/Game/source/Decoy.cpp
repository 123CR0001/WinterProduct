#include"Decoy.h"
#include"Player.h"
#include"ObjectServer.h"
#include"CapsuleComponent.h"
#include"AnimationComponent.h"

#include"ModeGame.h"
#include"ModeEffekseer.h"

Decoy::Decoy(Player* player)
	:ObjectBase(player->GetObjectServer(),false,"Decoy")
	,_capsule(NEW CapsuleComponent(this,1000))
	,_hitCnt(1)
	,_frameCnt(0)
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

	_handle = GetObjectServer()->GetGame()->GetModeEffekseer()->Play("Decoy", _pos, _eulerAngle);

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

	SetPosPlayingEffekseer3DEffect(_handle, _pos.x, _pos.y, _pos.z);

	SetRotationPlayingEffekseer3DEffect(_handle, _eulerAngle.x, _eulerAngle.y, _eulerAngle.z);

	//�Đ����I������
	if (_frameCnt == 30) {
		_handle = GetObjectServer()->GetGame()->GetModeEffekseer()->Play("Decoy", _pos, _eulerAngle);
		_frameCnt = 0;
	}

	_frameCnt++;

	return true;
}

bool Decoy::Render() {
	return true;
}