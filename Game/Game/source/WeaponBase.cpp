#include"WeaponBase.h"
#include"CharaBase.h"
#include"ObjectServer.h"
#include"ModeGame.h"
#include"../../../AppFrame/source/Collision/Segment.h"
#include"PhysWorld.h"
#include"CapsuleComponent.h"
#include"FrameComponent.h"

bool WeaponBase::_isAttackCollisionView = false;

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
	MV1RefreshCollInfo(_handle, _attachIndex);

	return true;
}

bool WeaponBase::Render() {

	MV1DrawModel(_handle);

	if (_isAttack) {
		//モデルに含まれる情報を持ってくる
		MV1_REF_POLYGONLIST list = MV1GetReferenceMesh(_handle, _attachIndex, TRUE);

		//頂点のインデックス
		MV1_REF_POLYGON* poly = list.Polygons;
		//頂点の位置情報
		MV1_REF_VERTEX* ver = list.Vertexs;

		//ポリゴン場を構築
		for (int a = 0; a < list.PolygonNum; a++) {
			DrawTriangle3D(
				ver[poly[a].VIndex[0]].Position,
				ver[poly[a].VIndex[1]].Position,
				ver[poly[a].VIndex[2]].Position,
				GetColor(255, 0, 0),
				FALSE
			);
		}
	}
	return true;
}

bool WeaponBase::IsHit(CharaBase* chara) {
	Segment seg = chara->GetCapsuleSegment();
	MV1_COLL_RESULT_POLY_DIM hit = MV1CollCheck_Capsule(
		_handle,
		MV1SearchFrame(_handle,"AttackCollision"),
		DxConverter::VecToDx(seg.start),
		DxConverter::VecToDx(seg.end),
		chara->GetRadian()
	);

	if (hit.HitNum > 0) { return true; }
	return false;
}