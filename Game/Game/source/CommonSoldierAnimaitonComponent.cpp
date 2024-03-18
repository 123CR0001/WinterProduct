#include"CommonSoldierAnimaitonComponent.h"
#include"CommonSoldier.h"
#include"AIComponent.h"
#include"MoveComponent.h"
#include"AIState.h"

CommonSoldierAnimationComponent::CommonSoldierAnimationComponent(CommonSoldier* owner, int order)
	:AnimationComponent(owner, order)
	, _csOwner(owner)
{


	_closeMaxTime = 12.f;

}

CommonSoldierAnimationComponent::~CommonSoldierAnimationComponent() {

}

bool CommonSoldierAnimationComponent::Process() {

	//�A�j���[�V�����̑J�ڏ������L�q



	//AI�̃X�e�[�g���ɕύX�����邩
	const char* name = _csOwner->GetAIComponent()->GetCurrentState()->GetName();


	if(name == "Attack") {
		ChangeAnimation("Shoot");
	}
	else if(name == "LookAround") {
		ChangeAnimation("LookAround");
	}
	else if(name == "LoseSight"){ ChangeAnimation("LoseSight"); }
	else if(name == "Panic") { ChangeAnimation("Shoot"); }
	else if(name == "Death") { ChangeAnimation("Death"); }
	else if(name == "Discovery") { ChangeAnimation("Discovery"); }
	else if(_csOwner->GetMoveComponent()->GetSpeed() > 0.01f || _csOwner->GetMoveComponent()->GetRatateSpeed().Length() > 0.01f) {
		ChangeAnimation("Walk");
	}
	else {
		ChangeAnimation("Idle");
	}
	
	AnimationComponent::Process();


	return true;
}