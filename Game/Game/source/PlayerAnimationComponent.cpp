#include"PlayerAnimationComponent.h"
#include"Player.h"

PlayerAnimationComponent::PlayerAnimationComponent(Player* owner, int order)
	:AnimationComponent(owner, order)
	, _pOwner(owner)
{

	_closeMaxTime = 3.0f;
}

PlayerAnimationComponent::~PlayerAnimationComponent() {

}

bool PlayerAnimationComponent::Process() {

	switch(_pOwner->GetState()) {
	case Player::ACTION_STATE::kIdle:
		ChangeAnimation("Idle");
		break;
	case Player::ACTION_STATE::kWalk:
		ChangeAnimation("run");
		break;
	case Player::ACTION_STATE::kAttack:
		ChangeAnimation("Attack");
		break;
	case Player::ACTION_STATE::kAttack2:
		ChangeAnimation("Attack2");
		break;
	case Player::ACTION_STATE::kAttack3:
		ChangeAnimation("Attack3");
		break;
	case Player::ACTION_STATE::kSilent:
		ChangeAnimation("Idle");
		break;
	case Player::ACTION_STATE::kSilentWalk:
		ChangeAnimation("StealthWalk");
		break;
	case Player::ACTION_STATE::kDeath:
		ChangeAnimation("Dead");
		break;
	case Player::ACTION_STATE::kClear:
		ChangeAnimation("Clear");
		break;
	}

	AnimationComponent::Process();

	return true;
}