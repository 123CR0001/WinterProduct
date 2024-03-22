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
	{
		auto physWorld = _owner->GetObjectServer()->GetPhysWorld();

		for (auto iter = physWorld->GetFrameComponent().begin(); iter != physWorld->GetFrameComponent().end(); ++iter) {

			if ((*iter)->GetOwner() == _owner) { continue; }//�����Ƃ͔�������Ȃ� 
			//�o�^���ꂢ�Ă��閼�O�Ȃ�A�X�L�b�v
			if (std::find(_skipNames.begin(), _skipNames.end(), (*iter)->GetOwner()->GetName()) != _skipNames.end()) { continue; }

			auto  hitObj = MV1CollCheck_Line(
				(*iter)->GetOwner()->GetHandle(),
				(*iter)->GetOwner()->GetAttachIndex(),
				DxConverter::VecToDx(_owner->GetPos()),
				DxConverter::VecToDx(_owner->GetPos() - Vector3(0.f, 1000.f, 0.f))
			);

			if (hitObj.HitFlag) {
				_owner->SetPos(DxConverter::DxToVec(hitObj.HitPosition) + Vector3(0.f,1.f,0.f));
			}
		}
	}


	//�I�u�W�F�N�g�Ƃ̉��o����
	int i = 0;

	_oldResult = _result;

	_result = PhysWorld::CollisionDetectionResult{};

	while (1) {

		PhysWorld::CollisionDetectionResult result = GetOverlapResult();
		
		if (result.isHit) {
			_owner->AddPos(result.item.pushVec);
			_result = result;
		}
		else { 
			break;
		}
		i++;
		//20��ȏ㔻���������Abreak
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
		if ((*iter)->GetOwner() == _owner) { continue; }//�����Ƃ͔�������Ȃ� 

		//�o�^���ꂢ�Ă��閼�O�Ȃ�A�X�L�b�v
		if (std::find(_skipNames.begin(), _skipNames.end(), (*iter)->GetOwner()->GetName()) != _skipNames.end()) { continue; }

		auto otherCap = (*iter)->GetCapsule();
		auto thisCap = GetCapsule();
		Segment resultSeg;
		if (Collision::Intersection(otherCap, thisCap, &resultSeg)) {

			//�����o�������������̂��A�ŏ��ɂԂ������J�v�Z��
			if (result.item.pushDistanceSquare < (otherCap.radius + thisCap.radius) - resultSeg.Length()) {
				result.isHit = true;

				Vector3 pushVec(Vector3(resultSeg.start + (resultSeg.Vector().Normalize() * (thisCap.radius + otherCap.radius))) - resultSeg.end);

				PhysWorld::CollisionDetectionItem item =
				{
					Vector3(resultSeg.start + (resultSeg.Vector().Normalize() * (thisCap.radius + otherCap.radius + 0.1f))) - resultSeg.end,
					pushVec.LengthSquare(),
					(*iter)->GetOwner(),
					Vector3(0.f,0.f,0.f),
					pushVec.Normalize()
				};
				result.item = item;
			}
			
		}
	}

	for (auto iter = physWorld->GetFrameComponent().begin(); iter != physWorld->GetFrameComponent().end(); ++iter) {

		if ((*iter)->GetOwner() == _owner) { continue; }//�����Ƃ͔�������Ȃ� 
		//�o�^���ꂢ�Ă��閼�O�Ȃ�A�X�L�b�v
		if (std::find(_skipNames.begin(), _skipNames.end(), (*iter)->GetOwner()->GetName()) != _skipNames.end()) { continue; }

		MV1_COLL_RESULT_POLY_DIM  hitObj = MV1CollCheck_Capsule(
			(*iter)->GetOwner()->GetHandle(),
			(*iter)->GetOwner()->GetAttachIndex(),
			DxConverter::VecToDx(GetCapsule().seg.start),
			DxConverter::VecToDx(GetCapsule().seg.end),
			_radius
		);

		//�Ԃ�������
		if (hitObj.HitNum > 0) {

			result.isHit = true;

			//�J�v�Z���ƃ|���S���̋���
			float dist = 9999999999.f;

			Segment capsuleSeg(GetCapsule().seg);

			//�|���S��(��)��̍ŋߓ_
			Vector3 polyLatestPoint;

			//������̍ŋߓ_
			Vector3 segLatestPoint;

			//�Ԃ������|���S���̖@���x�N�g��
			Vector3 normal;

			//�Ԃ������|���S���̐������J��Ԃ�
			for (int a = 0; a < hitObj.HitNum; a++) {
				Polygon3D detection = Polygon3D(
					DxConverter::DxToVec(hitObj.Dim[a].Position[0]),
					DxConverter::DxToVec(hitObj.Dim[a].Position[1]),
					DxConverter::DxToVec(hitObj.Dim[a].Position[2])
				);

				//�����ƃ|���S���̍ŋߓ_�⋗���𒲂ׂ�
				SEGMENT_TRIANGLE_RESULT result = Collision::SegmentPolygonAnalyze(detection, capsuleSeg);

				//�����ƃ|���S���̍ŋߓ_��2��̋���
				float distSqaure = result.Seg_Tri_MinDist_Square;

				//�J�v�Z���̐����ƍł��߂��|���S���̏���ێ�
				if (distSqaure < dist) {
					dist = distSqaure;
					polyLatestPoint = DxConverter::DxToVec(result.Tri_MinDist_Pos);
					segLatestPoint = DxConverter::DxToVec(result.Seg_MinDist_Pos);
					normal = DxConverter::DxToVec(VSub(result.Tri_MinDist_Pos, result.Seg_MinDist_Pos)).Normalize();
				}
			}

			//�ŋߓ_���m�ł̃x�N�g��
			Vector3 latestVec(segLatestPoint - polyLatestPoint);

			//�����o�������x�N�g��
			Vector3 push = latestVec.Normalize();

			//�����o���x�N�g���̍ő�l
			push *= (_radius + 0.5f);

			//�����o���x�N�g���̍ő�l����ŋߓ_���m�ł̃x�N�g��������
			//����ŁA�J�v�Z���ƃ|���S���̂��傤�Ǐd�Ȃ����������A�����o���x�N�g�����Z�o�ł���
			push -= latestVec;

			//�J�v�Z�����m�ł̉����o���x�N�g���̒������A�Z����
			//�Z���ƃJ�v�Z��������ɂԂ��������ƂɂȂ�
			if (result.item.pushDistanceSquare < Vector3(push).LengthSquare()) {
				PhysWorld::CollisionDetectionItem item =
				{
					Vector3(push),
					Vector3(push).LengthSquare(),
					(*iter)->GetOwner(),
					Vector3(0.f,0.f,0.f),
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
	//�L�����N�^�[�̈ʒu�͊�{�I�ɑ�������
//���̂��߁A
	Vector3 diff(_diff);
	diff.y += _radius;

	Vector3 capPos(_owner->GetPos() + diff);

	//�J�v�Z���R���W�����̎n�_����I�_��Y�����������ł����\���ł��Ȃ�
	return Capsule(
		capPos,
		capPos + Vector3(Vector3(0.f, 1.f, 0.f) * _length),
		_radius
	);
}