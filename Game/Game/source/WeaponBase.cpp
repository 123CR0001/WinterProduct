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
	
	if(_equippedChara){
		// �t���[��������t���[���ԍ����擾����
		int FrameIndex = MV1SearchFrame(_equippedChara->GetHandle(), _frameName.c_str());

		//���W��p�x�A�g�k�́A�����ł̓��[�J���Ƃ��Ĉ���
		MATRIX matrix = MGetIdent();
		matrix = MGetScale(DxConverter::VecToDx(_scale));
		matrix = MMult(matrix, MGetRotX(_eulerAngle.x));
		matrix = MMult(matrix, MGetRotX(_eulerAngle.y));
		matrix = MMult(matrix, MGetRotZ(_eulerAngle.z));
		matrix = MMult(matrix, MGetTranslate(DxConverter::VecToDx(_pos)));
		matrix = MMult(matrix, MV1GetFrameLocalWorldMatrix(_equippedChara->GetHandle(), FrameIndex));
		MV1SetMatrix(_handle, matrix);
	}

	//�����蔻��̍X�V
	MV1RefreshCollInfo(_handle, _attachIndex);

	return true;
}

bool WeaponBase::Render() {

	MV1DrawModel(_handle);

	return true;
}