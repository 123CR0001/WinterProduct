#include"AILookAround.h"
#include"AIComponent.h"
#include"ObjectBase.h"

#include"ObjectServer.h"
#include"PhysWorld.h"

#include"ModeGame.h"
#include"LightsOut.h"

AILookAround::AILookAround(AIComponent* owner)
	:AIState(owner)
	,_frameCnt(0)
{

}

AILookAround::~AILookAround() {

}

void AILookAround::OnEnter() {
	_frameCnt = 120;
}

void AILookAround::OnExist() {

}

bool AILookAround::Process() {
	_frameCnt--;

	if (_frameCnt <= 0) {
		_owner->ChangeState("BackPatrol");
	}

	//BackPatrol�͍ŏ��ɓo�^���ꂽ���W�ɖ߂�̂ŁAPatrol �� LookAround �� Chase(��������CheckPoint) �� LookAround �� BackPatrol�ł��APatrol�ŋL�^���ꂽ���W�ɖ߂�

	//���������������H
	{
		Vector3 p;
		if (_owner->GetOwner()->GetObjectServer()->GetPhysWorld()->IsHear(_owner->GetOwner(), &p)) {
			_owner->AddPoint("MoveTo", p);
			_owner->ChangeState("Discovery");
			_owner->AddPoint("BackPatrolGoal", _owner->GetOwner()->GetPos());
		}
	}

	//�o�^���Ă��閼�O�Ɠ������O�����I�u�W�F�N�g�����E�ɓ��ꂽ��
	{
		auto objects = _owner->GetOwner()->GetObjectServer()->GetObjects();
		std::vector<std::string>names;

		names.emplace_back("player");
		names.emplace_back("Decoy");

		for (int a = 0; a < objects.size(); a++) {

			//���E�ɓ����Ă��Ȃ�
			if (!_owner->IsFound(objects[a])) { continue; }

			for (int b = 0; b < names.size(); b++) {
				if (objects[a]->GetName() == names[b]) {
					//�ǂ�������I�u�W�F�N�g�̃A�h���X��o�^
					_owner->SetChaseObject(objects[a]);
					//AIState��ύX
					_owner->AddPoint("MoveTo", objects[a]->GetPos());
					_owner->ChangeState("Discovery");
					_owner->AddPoint("BackPatrolGoal", _owner->GetOwner()->GetPos());
					break;
				}
			}
		}
	}

	//LightsOut�ɂȂ�����,AIBlindWalk�ɕύX
	if(!_owner->GetOwner()->GetObjectServer()->GetGame()->GetLightsOut()->IsUse()) {
		_owner->ChangeState("BlindWalk");
	}



	return true;
}