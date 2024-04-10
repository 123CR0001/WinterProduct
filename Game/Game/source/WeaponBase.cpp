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
	//武器はフレームで当たり判定をしないので除外
	_frame->DeletePhysWorldThis();
}

WeaponBase::~WeaponBase() { }

bool WeaponBase::Initialize() {

	return true;
}

bool WeaponBase::Process() {
	ObjectBase::Process();
	
	if(_equippedChara){
		// フレーム名からフレーム番号を取得する
		int FrameIndex = MV1SearchFrame(_equippedChara->GetHandle(), _frameName.c_str());

		//座標や角度、拡縮は、ここではローカルとして扱う
		MATRIX matrix = MGetIdent();
		matrix = MGetScale(DxConverter::VecToDx(_scale));
		matrix = MMult(matrix, MGetRotX(_eulerAngle.x));
		matrix = MMult(matrix, MGetRotX(_eulerAngle.y));
		matrix = MMult(matrix, MGetRotZ(_eulerAngle.z));
		matrix = MMult(matrix, MGetTranslate(DxConverter::VecToDx(_pos)));
		matrix = MMult(matrix, MV1GetFrameLocalWorldMatrix(_equippedChara->GetHandle(), FrameIndex));
		MV1SetMatrix(_handle, matrix);
	}

	//当たり判定の更新
	MV1RefreshCollInfo(_handle, _attachIndex);

	return true;
}

bool WeaponBase::Render() {

	MV1DrawModel(_handle);

	return true;
}