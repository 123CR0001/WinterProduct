#include"AIBackPatrol.h"
#include"AIChasePlayer.h"
#include"AIComponent.h"
#include"Player.h"
#include"CommonSoldier.h"
#include"ObjectServer.h"
#include"appframe.h"
#include<algorithm>
#include"ModeGame.h"

AIBackPatrol::AIBackPatrol(AIComponent* owner)
	:AIState(owner)
	,_pointsNum(0)
{

}

AIBackPatrol::~AIBackPatrol(){}

void AIBackPatrol::OnEnter() {
	//_points�̃T�C�Y��0�̏ꍇ�́A��������n�_�ɖ߂��Ă����Ɣ��f
	if (_owner->GetPoints(GetName()).size() == 0) {
		_owner->ChangeState("Patrol");
	}
}
void AIBackPatrol::OnExist() {
	if (_owner->GetPoints(GetName()).size() == 0) { return; }
	for (int a = 0; a < _owner->GetPoints(GetName()).size(); a++) {
		_owner->GetPoints(GetName()).erase(_owner->GetPoints(GetName()).begin());
	}
}

bool AIBackPatrol::Process() {
	//Server��_enemys�ɓo�^����Ă��邩
	auto iter = std::find(_owner->GetOwner()->GetObjectServer()->GetCommonSoldiers().begin(),
		_owner->GetOwner()->GetObjectServer()->GetCommonSoldiers().end(),
		_owner->GetOwner()
	);

	//����Ă�����A����������
	if (iter == _owner->GetOwner()->GetObjectServer()->GetCommonSoldiers().end()) { return false; }

	//�v���C���[���������ɖڕW�n�_�ɂ����珄�񃋁[�g�ɖ߂�Patrol�ɐ؂�ւ�
	if ((*iter)->MoveRoute(_owner->GetPoints(GetName()), _pointsNum)) {
		_owner->ChangeState("Patrol");
	}
	//�v���C���[����������ACheckPlayer�ɐ؂�ւ�
	if ((*iter)->IsPlayerFound()) {
		_owner->ChangeState("ChasePlayer");
	}

	//LightsOut�ɂȂ�����,AIBlindWalk�ɕύX
	if (ModeServer::GetInstance()->IsAdd("LightsOut")) {
		_owner->ChangeState("BlindWalk");
	}

	return true;
}