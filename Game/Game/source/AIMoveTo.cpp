#include"AIMoveTo.h"
#include"AIComponent.h"
#include"ObjectServer.h"
#include"appframe.h"
#include"CommonSoldier.h"
#include"Player.h"
#include"ModeGame.h"
#include"LightsOut.h"
AIMoveTo::AIMoveTo(AIComponent* owner)
	:AIState(owner)
	, _pointsNum(0)
	, _frameCnt(0)
{
}
AIMoveTo::~AIMoveTo() {}
void AIMoveTo::OnEnter() {
	if(_owner->GetPoints(GetName()).empty()) { _owner->ChangeState("LookAround"); return; }
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
		_owner->ChangeState("LookAround");
		return;
	}
	_oldCheckPoint = checkPoint;
	_frameCnt = 0;
}
void AIMoveTo::OnExist() {
}
bool AIMoveTo::Process() {
	//��������������
	bool isFound = false;
	if(_owner->GetPoints(GetName()).size() == 0) {
		_owner->ChangeState("LookAround");
	}
	//�ړ��@�Ō�̍��W�܂œ��B������A����o�H�ɖ߂�
	if(!_owner->GetPoints(GetName()).back().Equal(_owner->GetOwner()->GetPos(), 20.f)) {
		//�ړ�
		_owner->MoveTo(_owner->GetPoints(GetName()), _pointsNum);
	}
	else {
		if(!isFound) {
			_owner->ChangeState("LookAround");
		}
	}
	//�o�^���Ă��閼�O�Ɠ������O�����I�u�W�F�N�g�����E�ɓ��ꂽ��
	{
		auto objects = _owner->GetOwner()->GetObjectServer()->GetObjects();
		std::vector<std::string>names;
		names.emplace_back("player");
		names.emplace_back("Decoy");
		for(int a = 0; a < objects.size(); a++) {
			//���E�ɓ����Ă��Ȃ�
			if(!_owner->IsFound(objects[a])) { continue; }
			for(int b = 0; b < names.size(); b++) {
				if(objects[a]->GetName() == names[b]) {
					//�ړI�n���X�V
					if(_oldCheckPoint != objects[a]->GetPos()) {
						_oldCheckPoint = _owner->GetPoints(GetName()).back();
						_owner->AddPoint(GetName(), objects[a]->GetPos());
						_owner->ChangeState(GetName());
					}
					isFound = true;
					break;
				}
			}
		}
	}
	//���������������H
	{
		Vector3 p;
		if(_owner->GetOwner()->GetObjectServer()->GetPhysWorld()->IsHear(_owner->GetOwner(), &p)) {
			_owner->AddPoint("MoveTo", p);
			_owner->ChangeState("Discovery");
			_owner->AddPoint("BackPatrolGoal", _owner->GetOwner()->GetPos());
		}
	}
	//LightsOut�ɂȂ�����,AIBlindWalk�ɕύX
	if(!_owner->GetOwner()->GetObjectServer()->GetGame()->GetLightsOut()->IsUse()) {
		_owner->ChangeState("BlindWalk");
	}
	return true;
}