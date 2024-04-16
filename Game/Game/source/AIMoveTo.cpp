#include"AIMoveTo.h"
#include"AIComponent.h"
#include"ObjectServer.h"
#include"appframe.h"
#include"CommonSoldier.h"
#include"Player.h"
#include"ModeGame.h"
#include"LightsOut.h"

constexpr int INTERVAL = 300;

AIMoveTo::AIMoveTo(AIComponent* owner)
	:AIState(owner)
	, _pointsNum(0)
	, _interval(0)
{
}
AIMoveTo::~AIMoveTo() {}
void AIMoveTo::OnEnter() {
	if(_owner->GetPoints(GetName()).empty()) { _owner->ChangeState("LoseSight"); return; }
	//��ԍŌ�ɒǉ����ꂽ���W�֌�����
	Vector3 checkPoint = _owner->GetPoints(GetName()).back();
	//�̂̃��[�g�͎̂Ă�
	_pointsNum = 0;
	_owner->GetPoints(GetName()).clear();
	//����AIState����������AIComponent����������ObjectBase����������Server
	auto server = _owner->GetOwner()->GetObjectServer();
	auto owner = _owner->GetOwner();
	auto navi = _owner->GetOwner()->GetObjectServer()->GetNavi();
	auto ownerPos = owner->GetPos();
	//�ŒZ�o�H
	navi->BFS(ownerPos, checkPoint, _owner->GetPoints(GetName()));
	if(_owner->GetPoints(GetName()).empty()) {
		_owner->ChangeState("LoseSight");
		return;
	}
	_oldCheckPoint = checkPoint;
	_interval = INTERVAL;
}
void AIMoveTo::OnExist() {
}
bool AIMoveTo::Process() {
	//��������������
	bool isFound = false;
	if(_owner->GetPoints(GetName()).size() == 0) {
		_owner->ChangeState("LoseSight");
	}
	//�ړ��@�Ō�̍��W�܂œ��B������A����o�H�ɖ߂�
	if(!Vector3::Equal(_owner->GetPoints(GetName()).back(),_owner->GetOwner()->GetPos(), 20.f)) {
		//�ړ�
		_owner->MoveTo(_owner->GetPoints(GetName()), _pointsNum);
	}
	else {
		if(!isFound) {
			_owner->ChangeState("LoseSight");
		}
	}

	//�o�^���Ă��閼�O�Ɠ������O�����I�u�W�F�N�g�����E�ɓ��ꂽ��
	{
		auto object = _owner->IsFound();

		if (object) {
			//AIState��ύX
			_owner->AddPoint("MoveTo", object->GetPos());
			_owner->ChangeState("Discovery");
			_owner->AddPoint("BackPatrolGoal", _owner->GetOwner()->GetPos());
		}
	}

	//��莞�Ԍo�߂�����A�����������������m�F
	if(_interval == 0) {
		Vector3 p;
		//���������������H
		if(_owner->GetOwner()->GetObjectServer()->GetPhysWorld()->IsHear(_owner->GetOwner(), &p)) {
			//���̔������̍��W��o�^
			_owner->AddPoint("MoveTo", p);

			//��������������ADiscovery�ɕύX
			_owner->ChangeState("Discovery");

			//��������W��߂���W�Ƃ��ēo�^
			_owner->AddPoint("BackPatrolGoal", _owner->GetOwner()->GetPos());
		}
	}
	//LightsOut�ɂȂ�����,AIBlindWalk�ɕύX
	if(!_owner->GetOwner()->GetObjectServer()->GetGame()->GetLightsOut()->IsUse()) {
		_owner->ChangeState("BlindWalk");
	}

	//�C���^�[�o���̍X�V
	if(_interval > 0) {
		_interval--;
	}
	return true;
}