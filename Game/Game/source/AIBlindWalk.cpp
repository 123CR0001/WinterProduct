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
	//移動する距離
	_moveDist = 1100.f;

	//どちらに回転するか
	if (rand() % 2) { _rotateSpeed *= -1; }

	//移動する最大量
	_rotateAngle = rand() % 360;

	_waitCnt = 0;

	_moveSpeed = 2.f;

}

void AIBlindWalk::OnExist(){}

bool AIBlindWalk::Process() {

	//移動速度
	float moveSpeed = _moveSpeed;

	//音が聞こえたか？
	if (_owner->GetOwner()->GetObjectServer()->GetPhysWorld()->IsHear(_owner->GetOwner())) {
		if (rand() % 2) { _owner->ChangeState("Panic"); }
		else { _moveSpeed = 2.f * 2.f; }
	}

	//一定時間止まる
	if (_waitCnt > 0) { _waitCnt--; return true; }

	//移動する距離は0ではないか
	if (_moveDist < 0.01f) { return true; }

	//回転
	if (_rotateAngle > 0) { 
		const Vector3 addAngle(0.f, DegToRad(static_cast<float>(_rotateSpeed)), 0.f);
		_owner->GetOwner()->AddEulerAngle(addAngle);
	}
	//回転したら、回転量を減らす
	_rotateAngle -= abs(_rotateSpeed);

	//移動
	_owner->GetOwner()->AddPos(_owner->GetOwner()->GetForward() * moveSpeed);

	_moveDist -= moveSpeed;

	//移動距離が0になった
	if (_moveDist < 0.001f) { _owner->ChangeState(GetName()); }

	//////オブジェクトにぶつかったら、一定時間止まる。その後、90度回転して、別方向に進む
	//if ((*iter)->GetCapsuleComponent()->GetOverlapResult().isHit && _rotateAngle <= 0) {
	//	_rotateAngle = 90;
	//	_waitCnt = 60;
	//}
		
	return true;
}