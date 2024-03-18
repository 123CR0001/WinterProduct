#include"WeaponBase.h"
#include"CharaBase.h"
#include"ObjectServer.h"
#include"ModeGame.h"
#include"appframe.h"
#include"PhysWorld.h"
#include"CapsuleComponent.h"
#include"FrameComponent.h"

WeaponBase::WeaponBase(CharaBase* chara)
	:ObjectBase(chara->GetObjectServer())
	,_equippedChara(chara)
	,_isAttack(false)
	,_frame(NEW FrameComponent(this))
{ 
	//����̓t���[���œ����蔻������Ȃ��̂ŏ��O
	_frame->DeletePhysWorldThis();
}

WeaponBase::~WeaponBase() { }

bool WeaponBase::Initialize() {

	return true;
}

bool WeaponBase::Process() {
	ObjectBase::Process();
	MV1RefreshCollInfo(_handle, _attachIndex);

	return true;
}

bool WeaponBase::Render() {

	MV1DrawModel(_handle);

	return true;
}