#include"CapsuleComponent.h"
#include"FrameComponent.h"
#include"ObjectBase.h"
#include"PhysWorld.h"
#include"ObjectServer.h"
#include<algorithm>
CapsuleComponent::CapsuleComponent(ObjectBase* owner,int order)
	:Component(owner,order)
{
	owner->GetObjectServer()->GetPhysWorld()->GetCapsuleComponent().emplace_back(this);
}

CapsuleComponent::~CapsuleComponent() {
	auto world = _owner->GetObjectServer()->GetPhysWorld();
	auto iter = std::find(
		world->GetCapsuleComponent().begin(),
		world->GetCapsuleComponent().end(),
		this);
	if (iter != world->GetCapsuleComponent().end()) {
		world->GetCapsuleComponent().erase(iter);
	}
}

bool CapsuleComponent::Process() {
	//オブジェクトとの押出処理
	int i = 0;

	//一度空にする
	_result = PhysWorld::CollisionDetectionResult{};

	while (1) {
		PhysWorld::CollisionDetectionResult result = GetOverlapResult();

		if (result.isHit) {
			_owner->AddPos(result.item.pushVec);
			_result = result;
		}
		else { break; }
		i++;
		//20回以上判定をしたら、break
		if (i > 20) {
			break;
		}
	}
	return true;
}

PhysWorld::CollisionDetectionResult CapsuleComponent::GetOverlapResult() {
	auto physWorld = _owner->GetObjectServer()->GetPhysWorld();
	auto capComs = physWorld->GetCapsuleComponent();

	PhysWorld::CollisionDetectionResult result;
	result.isHit = false;

	for (auto iter = capComs.begin(); iter != capComs.end(); ++iter) {
		if ((*iter)->GetOwner() == _owner) { continue; }//自分とは判定をしない 

		auto otherCap = (*iter)->GetCapsule();
		auto thisCap = GetCapsule();
		Segment resultSeg;
		if (Collision::Intersection(otherCap, thisCap, &resultSeg)) {

			//押し出す距離が長いのが、最初にぶつかったカプセル
			if (result.item.pushDistanceSqaure < thisCap.radius + otherCap.radius) {
				result.isHit = true;

				Vector3D pushVec(Vector3D(resultSeg.start + (resultSeg.Vector().Normalize() * (thisCap.radius + otherCap.radius))) - resultSeg.end);

				PhysWorld::CollisionDetectionItem item =
				{
					Vector3D(resultSeg.start + (resultSeg.Vector().Normalize() * (thisCap.radius + otherCap.radius + 0.1f))) - resultSeg.end,
					pushVec.LengthSquare(),
					(*iter)->GetOwner(),
					Vector3D(0.f,0.f,0.f),
					pushVec.Normalize()
				};
				result.item = item;
			}
			
		}
	}

	for (auto iter = physWorld->GetFrameComponent().begin(); iter != physWorld->GetFrameComponent().end(); ++iter) {

		if ((*iter)->GetOwner() == _owner) { continue; }//自分とは判定をしない 

		MV1_COLL_RESULT_POLY_DIM  hitObj = MV1CollCheck_Capsule(
			(*iter)->GetOwner()->GetHandle(),
			(*iter)->GetOwner()->GetAttachIndex(),
			DxConverter::VecToDx(GetCapsule().seg.start),
			DxConverter::VecToDx(GetCapsule().seg.end),
			_radius
		);

		//ぶつかったか
		if (hitObj.HitNum > 0) {

			result.isHit = true;

			//カプセルとポリゴンの距離
			float dist = 9999999999.f;

			Segment capsuleSeg(GetCapsule().seg);

			//ポリゴン(面)上の最近点
			Vector3D polyLatestPoint;

			//線分上の最近点
			Vector3D segLatestPoint;

			//ぶつかったポリゴンの法線ベクトル
			Vector3D normal;

			//ぶつかったポリゴンの数だけ繰り返す
			for (int a = 0; a < hitObj.HitNum; a++) {
				Polygon3D detection = Polygon3D(
					DxConverter::DxToVec(hitObj.Dim[a].Position[0]),
					DxConverter::DxToVec(hitObj.Dim[a].Position[1]),
					DxConverter::DxToVec(hitObj.Dim[a].Position[2])
				);

				//線分とポリゴンの最近点や距離を調べる
				SEGMENT_TRIANGLE_RESULT result = Collision::SegmentPolygonAnalyze(detection, capsuleSeg);

				//線分とポリゴンの最近点や2乗の距離
				float distSqaure = result.Seg_Tri_MinDist_Square;

				//カプセルの線分と最も近いポリゴンの情報を保持
				if (distSqaure < dist) {
					dist = distSqaure;
					polyLatestPoint = DxConverter::DxToVec(result.Tri_MinDist_Pos);
					segLatestPoint = DxConverter::DxToVec(result.Seg_MinDist_Pos);
					normal = DxConverter::DxToVec(hitObj.Dim[a].Normal);
				}
			}

			//最近点同士でのベクトル
			Vector3D latestVec(segLatestPoint - polyLatestPoint);

			//押し出す向きベクトル
			Vector3D push = latestVec.Normalize();

			//押し出すベクトルの最大値
			push *= (_radius + 0.5f);

			//押し出すベクトルの最大値から最近点同士でのベクトル分引く
			//これで、カプセルとポリゴンのちょうど重なった分だけ、押し出すベクトルが算出できる
			push -= latestVec;

			//カプセル同士での押し出すベクトルの長さより、短いか
			//短いとカプセルよりも先にぶつかったことになる
			if (result.item.pushDistanceSqaure < Vector3D(push).LengthSquare()) {
				PhysWorld::CollisionDetectionItem item =
				{
					Vector3D(push),
					Vector3D(push).LengthSquare(),
					(*iter)->GetOwner(),
					Vector3D(0.f,0.f,0.f),
					normal
				};
				result.item = item;
			}
		}
		MV1CollResultPolyDimTerminate(hitObj);
	}



	return result;
}

Capsule CapsuleComponent::GetCapsule()const {
	//キャラクターの位置は基本的に足元から
//そのため、
	Vector3D diff(_diff);
	diff.y += _radius;

	Vector3D capPos(_owner->GetPos() + diff);

	//カプセルコリジョンの始点から終点をY軸成分だけでしか表現できない
	return Capsule(
		capPos,
		capPos + Vector3D(Vector3D(0.f, 1.f, 0.f) * _length),
		_radius
	);
}