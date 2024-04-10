#include"PlayerAnimationComponent.h"
#include"Player.h"

PlayerAnimationComponent::PlayerAnimationComponent(Player* owner, int order)
	:AnimationComponent(owner, order)
	, _pOwner(owner)
{

	_closeMaxTime = 3.0f;

	//ƒ‚ƒfƒ‹‚Ì“Ç‚Ýž‚Ý
	_owner->LoadModel("res/Chara/Owl_toDX/Owl.mv1");

	LoadAnimation("Idle", "mo_standby_01", 0);
	LoadAnimation("run", "mo_move_01", 0);
	LoadAnimation("StealthWalk", "mo_stealthwalk_01", 0);
	LoadAnimation("Attack", "mo_attack_01", 1);
	LoadAnimation("Attack2", "mo_attack_02", 1);
	LoadAnimation("Attack3", "mo_attack_03", 1);
	LoadAnimation("Dead", "mo_death_01", 1);
	LoadAnimation("Clear", "mo_standby_01", 0);
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