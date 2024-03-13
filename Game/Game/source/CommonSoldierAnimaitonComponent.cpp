#include"CommonSoldierAnimaitonComponent.h"
#include"CommonSoldier.h"
#include"AIComponent.h"
#include"MoveComponent.h"
#include"AIState.h"

CommonSoldierAnimationComponent::CommonSoldierAnimationComponent(CommonSoldier* owner, int order)
	:AnimationComponent(owner, order)
	, _csOwner(owner)
{
	//アニメーションを登録

	_csOwner->LoadModel("res/Chara/soldier_2_4_IK_A.mv1");

	LoadAnimation("Walk","mo_moveenemy_01",0);
	LoadAnimation("Death", "mo_deathenemy_01", 1);
	LoadAnimation("LookAround", "mo_losesight_01", 0);
	LoadAnimation("Shoot", "mo_shootingenemy_01", 0);
	LoadAnimation("Idle", "mo_standbyenemy_01", 0);
	LoadAnimation("Discovery", "mo_discovery_01", 0);

	_closeMaxTime = 12.f;

}

CommonSoldierAnimationComponent::~CommonSoldierAnimationComponent() {

}

bool CommonSoldierAnimationComponent::Process() {

	//アニメーションの遷移条件を記述



	//AIのステート名に変更があるか
	const char* name = _csOwner->GetAIComponent()->GetCurrentState()->GetName();


	if(name == "Attack") {
		ChangeAnimation("Shoot");
	}
	else if(name == "LookAround") {
		ChangeAnimation("LookAround");
	}
	else if(name == "Panic") { ChangeAnimation("Shoot"); }
	else if(name == "Death") { ChangeAnimation("Death"); }
	else if(name == "Discovery") { ChangeAnimation("Discovery"); }
	else if(_csOwner->GetMoveComponent()->GetSpeed() > 0.01f) {
		ChangeAnimation("Walk");
	}
	else {
		ChangeAnimation("Idle");
	}
	
	AnimationComponent::Process();


	return true;
}