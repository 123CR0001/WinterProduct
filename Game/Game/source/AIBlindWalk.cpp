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

	_moveSpeed = 2.f;

}

void AIBlindWalk::OnExist(){}

bool AIBlindWalk::Process() {

	//ˆÚ“®‘¬“x
	float moveSpeed = _moveSpeed;

	//‰¹‚ª•·‚±‚¦‚½‚©H
	if (_owner->GetOwner()->GetObjectServer()->GetPhysWorld()->IsHear(_owner->GetOwner())) {
		if (rand() % 2) { _owner->ChangeState("Panic"); }
		else { _moveSpeed = 2.f * 2.f; }
	}

	//ˆê’èŽžŠÔŽ~‚Ü‚é
	if (_waitCnt > 0) { _waitCnt--; return true; }

	//ˆÚ“®‚·‚é‹——£‚Í0‚Å‚Í‚È‚¢‚©
	if (_moveDist < 0.01f) { return true; }

	//‰ñ“]
	if (_rotateAngle > 0) { 
		const Vector3 addAngle(0.f, DegToRad(static_cast<float>(_rotateSpeed)), 0.f);
		_owner->GetOwner()->AddEulerAngle(addAngle);
	}
	//‰ñ“]‚µ‚½‚çA‰ñ“]—Ê‚ðŒ¸‚ç‚·
	_rotateAngle -= abs(_rotateSpeed);

	//ˆÚ“®
	_owner->GetOwner()->AddPos(_owner->GetOwner()->GetForward() * moveSpeed);

	_moveDist -= moveSpeed;

	//ˆÚ“®‹——£‚ª0‚É‚È‚Á‚½
	if (_moveDist < 0.001f) { _owner->ChangeState(GetName()); }

	//////ƒIƒuƒWƒFƒNƒg‚É‚Ô‚Â‚©‚Á‚½‚çAˆê’èŽžŠÔŽ~‚Ü‚éB‚»‚ÌŒãA90“x‰ñ“]‚µ‚ÄA•Ê•ûŒü‚Éi‚Þ
	//if ((*iter)->GetCapsuleComponent()->GetOverlapResult().isHit && _rotateAngle <= 0) {
	//	_rotateAngle = 90;
	//	_waitCnt = 60;
	//}
		
	return true;
}