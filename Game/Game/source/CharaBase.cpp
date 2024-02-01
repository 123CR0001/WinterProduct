#include"CharaBase.h"
#include"ObjectServer.h"
#include"ModeGame.h"
#include"AnimationConmponent.h"
#include"CapsuleComponent.h"
#include<algorithm>
#include"appframe.h"
#include"FrameComponent.h"

CharaBase::CharaBase(ObjectServer* server) 
	:ObjectBase(server)
	, _anim(new AnimationComponent(this))
	,_capsule(new CapsuleComponent(this))
{
	server->GetCharas().emplace_back(this);
	_capsule->SetMember(170.f, 40.f);

	//キャラはフレームで当たり判定をしないので除外
	DeleteComponent(_frame);
	_frame->DeletePhysWorldThis();

}

CharaBase::~CharaBase() {
	
}

bool CharaBase::ChangeState(std::string stateName) {
	return true;
}

bool CharaBase::Initialize() {
	ObjectBase::Initialize();

	_isStand = false;
	_motCnt = 0;

	_radian = 0;
	_colSubY = 0;	// コリジョン判定時のY補正(腰位置）

	return true;
}

bool CharaBase::Terminate(){

	ObjectBase::Terminate();

	auto iter = std::find(
		GetObjectServer()->GetCharas().begin(), GetObjectServer()->GetCharas().end(), this);
	if (iter != GetObjectServer()->GetCharas().end()) {
		GetObjectServer()->GetCharas().erase(iter);
	}

	return true;
}

bool CharaBase::IsHitMap(){
	auto server = GetObjectServer();

	//カプセル判定
	MV1_COLL_RESULT_POLY_DIM hit = MV1CollCheck_Capsule(
		server->GetNavigationHandle(),
		server->GetNavigationAttachIndex(),
		DxConverter::VecToDx(_pos + Vector3D(0, _radian, 0)),
		DxConverter::VecToDx(_pos + Vector3D(0, _radian + _colSubY, 0)),
		_radian
	);
	if (hit.Dim) {

		double dist = 9999999999;

		Vector3D under_pos(_pos);
		under_pos.y += _radian;
		Vector3D up_pos(_pos);
		up_pos.y += (_colSubY + _radian);

		Segment coll_seg(under_pos, up_pos);
		Vector3D pol_latest_point(0, 0, 0);
		Vector3D add_base_point(0, 0, 0);

		for (int a = 0; a < hit.HitNum; a++) {
			Polygon3D detection = Polygon3D(
				DxConverter::DxToVec(hit.Dim[a].Position[0]),
				DxConverter::DxToVec(hit.Dim[a].Position[1]),
				DxConverter::DxToVec(hit.Dim[a].Position[2])
			);

			SEGMENT_TRIANGLE_RESULT result = Collision::SegmentPolygonAnalyze(detection, coll_seg);

			double dist_com = result.Seg_Tri_MinDist_Square;

			if (dist_com < dist) {
				dist = dist_com;
				pol_latest_point = DxConverter::DxToVec(result.Tri_MinDist_Pos);
				add_base_point = DxConverter::DxToVec(result.Seg_MinDist_Pos);
			}
		}

		if (dist < _radian * _radian) {
			Vector3D latest_v(pol_latest_point - add_base_point);
			latest_v.Normalized();
			latest_v *= (_radian);
			Vector3D add((pol_latest_point - latest_v));
			_pos += (add - add_base_point);
		}

	}
	MV1CollResultPolyDimTerminate(hit);

	return true;
}

bool CharaBase::IsPushedObject(ObjectBase* obj) {

	MV1_COLL_RESULT_POLY hit = MV1CollCheck_Line(
		obj->GetHandle(),
		obj->GetAttachIndex(),
		DxConverter::VecToDx(_pos + Vector3D(0, 40, 0)),
		DxConverter::VecToDx(_pos + Vector3D(0, -20, 0))
	);
	if (hit.HitFlag) {
		// 当たった
		// 当たったY位置をキャラ座標にする
		_pos.y = hit.HitPosition.y;
	}

	MV1_COLL_RESULT_POLY_DIM  hitobj = MV1CollCheck_Capsule(
		obj->GetHandle(),
		obj->GetAttachIndex(),
		DxConverter::VecToDx(_pos + Vector3D(0, _radian, 0)),
		DxConverter::VecToDx(_pos + Vector3D(0, _radian + _colSubY, 0)),
		_radian
	);

	if (hitobj.Dim) {

		double dist = 9999999999;

		Vector3D under_pos(_pos);
		under_pos.y += _radian;
		Vector3D up_pos(_pos);
		up_pos.y += (_colSubY + _radian);

		Segment coll_seg(under_pos, up_pos);
		Vector3D pol_latest_point(0, 0, 0);
		Vector3D add_base_point(0, 0, 0);

		for (int a = 0; a < hitobj.HitNum; a++) {
			Polygon3D detection = Polygon3D(
				DxConverter::DxToVec(hitobj.Dim[a].Position[0]),
				DxConverter::DxToVec(hitobj.Dim[a].Position[1]),
				DxConverter::DxToVec(hitobj.Dim[a].Position[2])
			);

			SEGMENT_TRIANGLE_RESULT result = Collision::SegmentPolygonAnalyze(detection, coll_seg);

			double dist_com = result.Seg_Tri_MinDist_Square;

			if (dist_com < dist) {
				dist = dist_com;
				pol_latest_point = DxConverter::DxToVec(result.Tri_MinDist_Pos);
				add_base_point = DxConverter::DxToVec(result.Seg_MinDist_Pos);
			}
		}

		if (dist < _radian * _radian) {
			Vector3D latest_v(pol_latest_point - add_base_point);
			latest_v.Normalized();
			latest_v *= (_radian);
			Vector3D add((pol_latest_point - latest_v));
			_pos += (add - add_base_point);
		}
	}
	MV1CollResultPolyDimTerminate(hitobj);
	return true;
}

bool CharaBase::IsPushedChara(CharaBase* chara) {

	float sumRadian = _radian + chara->GetRadian();

	float sumRadianSq = sumRadian * sumRadian;

	Segment result;

	Collision::SegSegDist(chara->GetCapsuleSegment(), GetCapsuleSegment(), &result);

	//カプセル同士の押し出し
	if (result.LengthSquare() < sumRadianSq) {
		Vector3D pushVec(result.start + (result.Vector().Normalize() * sumRadian));
		_pos += (pushVec - result.end);
	}

	return true;
}

bool CharaBase::Render() {

	ModelMatrixSetUp();
	MV1DrawModel(_handle);

	return true;
}

void CharaBase::FixPos() {
	//オブジェクトとの押出処理
	int i = 0;
	while (1) {
		PhysWorld::CollisionDetectionResult result = _capsule->GetOverlapResult();

		if (result.isHit) {
			_pos += result.item.pushVec;
		}
		else { break; }
		i++;
		//20回以上判定をしたら、break
		if (i > 20) {
			break;
		}
	}
}