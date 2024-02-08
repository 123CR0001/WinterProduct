#include"AIBlindWalk.h"
#include"AIComponent.h"
#include"ObjectBase.h"
#include"CommonSoldier.h"
#include"ObjectServer.h"
#include"MoveComponent.h"
#include"CapsuleComponent.h"
#include<algorithm>

AIBlindWalk::AIBlindWalk(AIComponent* owner)
	:AIState(owner)
	,_rotateSpeed(1)
	,_rotateAngle(0)
	,_moveDist(0.f)
{

}

AIBlindWalk::~AIBlindWalk(){}

void AIBlindWalk::OnEnter(){
	//ˆÚ“®‚·‚é‹——£
	_moveDist = 1100.f;

	//‚Ç‚¿‚ç‚É‰ñ“]‚·‚é‚©
	if (rand() % 2) { _rotateSpeed *= -1; }

	//ˆÚ“®‚·‚éÅ‘å—Ê
	_rotateAngle = rand() % 360;

	_waitCnt = 0;
}

void AIBlindWalk::OnExist(){}

bool AIBlindWalk::Process() {

	//ˆê’èŽžŠÔŽ~‚Ü‚é
	if (_waitCnt > 0) { _waitCnt--; return true; }

	auto owner = _owner->GetOwner();
	auto iter = std::find(owner->GetObjectServer()->GetCommonSoldiers().begin(), owner->GetObjectServer()->GetCommonSoldiers().end(), owner);

	//“o˜^‚³‚ê‚Ä‚¢‚é‚©
	if (iter == owner->GetObjectServer()->GetCommonSoldiers().end()) { return false; }

	//ˆÚ“®‚·‚é‹——£‚Í0‚Å‚Í‚È‚¢‚©
	if (_moveDist < 0.01f) { return true; }

	//ˆÚ“®‘¬“xˆê’è
	float moveSpeed = 2.f;

	//MoveComponent‚ð—˜—p‚µ‚ÄA‰ñ“]
	if (_rotateAngle > 0) { (*iter)->GetMoveComponent()->SetRotateSpeed(DegToRad(static_cast<float>(_rotateSpeed))); }
	//‰ñ“]‚µ‚½‚çA‰ñ“]—Ê‚ðŒ¸‚ç‚·
	_rotateAngle -= abs(_rotateSpeed);

	(*iter)->GetMoveComponent()->SetMoveSpeed(moveSpeed);
	_moveDist -= moveSpeed;

	//ˆÚ“®‹——£‚ª0‚É‚È‚Á‚½
	if (_moveDist < 0.001f) { _owner->ChangeState(GetName()); }

	//ƒIƒuƒWƒFƒNƒg‚É‚Ô‚Â‚©‚Á‚½‚çAˆê’èŽžŠÔŽ~‚Ü‚éB‚»‚ÌŒãA90“x‰ñ“]‚µ‚ÄA•Ê•ûŒü‚Éi‚Þ
	if ((*iter)->GetCapsuleComponent()->GetOverlapResult().isHit && _rotateAngle <= 0) {
		_rotateAngle = 90;
		_waitCnt = 60;
	}
		
	return true;
}