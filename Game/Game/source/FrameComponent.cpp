#include"FrameComponent.h"
#include"ObjectBase.h"
#include"ObjectServer.h"
#include"PhysWorld.h"
#include<algorithm>
#include"CapsuleComponent.h"

FrameComponent::FrameComponent(ObjectBase* owner,int order)
	:Component(owner, order)
{
	//物理ワールドに登録
	_owner->GetObjectServer()->GetPhysWorld()->GetFrameComponent().emplace_back(this);
}

FrameComponent::~FrameComponent() {
	//物理ワールドから削除
	DeletePhysWorldThis();
}

bool FrameComponent::Process() {

	//前回の結果を保存
	_oldResult = _result;

	//判定
	_result = GetOverlapResult();

	return true;
}

PhysWorld::CollisionDetectionResult FrameComponent::GetOverlapResult() {

	auto physWorld = _owner->GetObjectServer()->GetPhysWorld();

	//衝突情報を保存
	PhysWorld::CollisionDetectionResult result;

	//1フレームに一回しか判定していない
	for (auto iter = physWorld->GetCapsuleComponent().begin(); iter != physWorld->GetCapsuleComponent().end(); ++iter) {

		MV1_COLL_RESULT_POLY_DIM hit = MV1CollCheck_Capsule(
			_owner->GetHandle(),
			_owner->GetAttachIndex(),
			DxConverter::VecToDx((*iter)->GetCapsule().seg.start),
			DxConverter::VecToDx((*iter)->GetCapsule().seg.end),
			(*iter)->GetCapsule().radius
		);

		//当たった
		if (hit.HitNum > 0) { 

			result.isHit = true;
			result.item._object = (*iter)->GetOwner();

			Vector3 polyLatestPoint(0.f, 0.f, 0.f);
			Vector3 segLatestPoint(0.f, 0.f, 0.f);

			Polygon3D detection = Polygon3D(
				DxConverter::DxToVec(hit.Dim[0].Position[0]),
				DxConverter::DxToVec(hit.Dim[0].Position[1]),
				DxConverter::DxToVec(hit.Dim[0].Position[2])
			);

			SEGMENT_TRIANGLE_RESULT seGTriresult = Collision::SegmentPolygonAnalyze(detection, (*iter)->GetCapsule().seg);

			//ポリゴン上で最も近い点を取得
			polyLatestPoint = DxConverter::DxToVec(seGTriresult.Tri_MinDist_Pos);

			//線分上で最も近い点を取得
			segLatestPoint = DxConverter::DxToVec(seGTriresult.Seg_MinDist_Pos);

			result.item.hitPosition = segLatestPoint;
			result.item.pushVec = Vector3(segLatestPoint - polyLatestPoint).Normalize();
		}

		MV1CollResultPolyDimTerminate(hit);
	}

	return result;
}

void FrameComponent::DeletePhysWorldThis() {

	//物理ワールドから削除
	auto world = _owner->GetObjectServer()->GetPhysWorld();

	auto iter = std::find(world->GetFrameComponent().begin(), world->GetFrameComponent().end(), this);
	if (iter != world->GetFrameComponent().end()) {
		world->GetFrameComponent().erase(iter);
	}
}