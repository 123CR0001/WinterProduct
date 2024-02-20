#include"CommonSoldierAnimaitonComponent.h"
#include"CommonSoldier.h"
#include"AIComponent.h"
#include"MoveComponent.h"
#include"AIState.h"

CommonSoldierAnimationComponent::CommonSoldierAnimationComponent(CommonSoldier* owner, int order)
	:AnimationComponent(owner, order)
	, _csOwner(owner)
{
	//�A�j���[�V������o�^

	_csOwner->LoadModel("res/Chara/soldier_2_4_IK_A.mv1");

	LoadAnimation("Walk","mo_moveenemy_01",0);
	LoadAnimation("Death", "mo_deathenemy_01", 1);
	LoadAnimation("LookAround", "mo_losesight_01", 0);

}

CommonSoldierAnimationComponent::~CommonSoldierAnimationComponent() {

}

bool CommonSoldierAnimationComponent::Process() {
	AnimationComponent::Process();

	//�A�j���[�V�����̑J�ڏ������L�q


	//AI�̃X�e�[�g���ɕύX�����邩
	if (_stateName != _csOwner->GetAIComponent()->GetCurrentState()->GetName()) {

		const char* name = _csOwner->GetAIComponent()->GetCurrentState()->GetName();

		if (name == "LookAround") {
			ChangeAnimation("LookAround");
		}
		else if (_csOwner->GetMoveComponent()->GetSpeed() >= 1.0f) {
			ChangeAnimation("Walk");
		}

	}
	_stateName = _csOwner->GetAIComponent()->GetCurrentState()->GetName();

	return true;
}