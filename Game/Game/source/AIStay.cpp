#include"AIStay.h"
#include"AIComponent.h"
#include"ObjectBase.h"
#include"ObjectServer.h"

AIStay::AIStay(AIComponent* owner)
	:AIState(owner)
	,_ownerEulerAngle(nullptr)
{

}

AIStay::~AIStay(){}

void AIStay::OnEnter() {
	if(!_ownerEulerAngle)_ownerEulerAngle = std::make_unique<Vector3D>(_owner->GetOwner()->GetEulerAngle());
}

void AIStay::OnExist() {

}

bool AIStay::Process() {

	//���������������H
	{
		Vector3D p;
		if(_owner->GetOwner()->GetObjectServer()->GetPhysWorld()->IsHear(_owner->GetOwner(), &p)) {
			_owner->AddPoint("CheckPoint", p);
			_owner->ChangeState("CheckPoint");
			_owner->AddPoint("BackPatrolGoal", _owner->GetOwner()->GetPos());
		}
	}

	//�o�^���Ă��閼�O�Ɠ������O�����I�u�W�F�N�g�����E�ɓ��ꂽ��
	{
		auto objects = _owner->GetOwner()->GetObjectServer()->GetObjects();
		std::vector<std::string>names;

		names.emplace_back("player");
		names.emplace_back("Decoy");

		for(int a = 0; a < objects.size(); a++) {

			for(int b = 0; b < names.size(); b++) {

				//�o�^�������O��
				if(objects[a]->GetName() != names[b]) { continue; }

				//���E�ɓ����Ă��邩
				if(_owner->IsFound(objects[a])) {
					//�ǂ�������I�u�W�F�N�g�̃A�h���X��o�^
					_owner->SetChaseObject(objects[a]);
					//AIState��ύX
					_owner->ChangeState("Chase");
					_owner->AddPoint("BackPatrolGoal", _owner->GetOwner()->GetPos());
					return true;

				}
			}
		}
	}

	//LightsOut�ɂȂ�����,AIBlindWalk�ɕύX
	if(ModeServer::GetInstance()->IsAdd("LightsOut")) {
		_owner->ChangeState("BlindWalk");
	}

	_owner->GetOwner()->SetEulerAngle(*_ownerEulerAngle);

	//const Vector3D vCross = Vector3D::Cross(_owner->GetOwner()->GetForward(), *_ownerForward);

	//if(fabsf(sinf(vCross.z)) < DegToRad(3)) { return true; }

	//if(vCross.z < 0.f) {
	//	_owner->GetOwner()->AddEulerAngle(Vector3D(0.f, -DegToRad(1), 0.f));
	//}
	//else {
	//	_owner->GetOwner()->AddEulerAngle(Vector3D(0.f, DegToRad(1), 0.f));
	//}

	return true;
}