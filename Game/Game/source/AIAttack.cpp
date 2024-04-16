#include"AIAttack.h"
#include"AIComponent.h"

#include"Player.h"
#include"ObjectBase.h"
#include"ObjectServer.h"

#include"appframe.h"
AIAttack::AIAttack(AIComponent* owner)
	:AIState(owner)
	,_rotateAngle(0.f)
{

}

AIAttack::~AIAttack(){}

void AIAttack::OnEnter(){




}

void AIAttack::OnExist(){}

bool AIAttack::Process() {

	//オーナーの前方ベクトル
	auto forward = _owner->GetOwner()->GetForward().Normalize();

	//プレイヤーの位置
	auto playerPos = _owner->GetOwner()->GetObjectServer()->GetPlayer()->GetPos();

	//プレイヤーとオーナーの位置の差
	auto playerToOwner = playerPos - _owner->GetOwner()->GetPos();

	//プレイヤーとオーナーの位置の差を正規化
	//プレイヤーからオーナーの向きべクトル
	playerToOwner.Normalized();
	
	//オーナーが向く角度
	_rotateAngle = Vector3::CrossAngleXZ(forward, playerToOwner);

	//回転角度が一定以下になったらtrueを返して、処理を終了
	if(fabsf(_rotateAngle) < DegToRad(10.f)) { return true; }

	//オーナーの角度
	auto ownerAngle = _owner->GetOwner()->GetEulerAngle();

	//回転角度が0より大きい場合、左に回転
	if(_rotateAngle > 0.f) {
		ownerAngle.y -= DegToRad(3.f);
		_owner->GetOwner()->SetEulerAngle(ownerAngle);
	}
	//回転角度が0より小さい場合、右に回転
	else {
		ownerAngle.y += DegToRad(3.f);
		_owner->GetOwner()->SetEulerAngle(ownerAngle);
	}

	return true;
}
