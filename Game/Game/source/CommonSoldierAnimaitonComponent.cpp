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
}

CommonSoldierAnimationComponent::~CommonSoldierAnimationComponent() {

}

bool CommonSoldierAnimationComponent::Process() {
	AnimationComponent::Process();

	//アニメーションの遷移条件を記述

	AnimationComponent* anim = _csOwner->GetAnimationComponent();

	//すでに変更したいアニメーションに、変更されていた場合は処理をしないので、条件式は遷移条件だけでいい

	if (_csOwner->GetAIComponent()->GetCurrentState()->GetName() == "Panic") {

	}
	else if (_csOwner->GetMoveComponent()->GetSpeed() >= 2.0f) {
		ChangeAnimation("");
	}

	return true;
}