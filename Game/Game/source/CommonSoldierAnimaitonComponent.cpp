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
}

CommonSoldierAnimationComponent::~CommonSoldierAnimationComponent() {

}

bool CommonSoldierAnimationComponent::Process() {
	AnimationComponent::Process();

	//�A�j���[�V�����̑J�ڏ������L�q

	AnimationComponent* anim = _csOwner->GetAnimationComponent();

	//���łɕύX�������A�j���[�V�����ɁA�ύX����Ă����ꍇ�͏��������Ȃ��̂ŁA�������͑J�ڏ��������ł���

	if (_csOwner->GetAIComponent()->GetCurrentState()->GetName() == "Panic") {

	}
	else if (_csOwner->GetMoveComponent()->GetSpeed() >= 2.0f) {
		ChangeAnimation("");
	}

	return true;
}