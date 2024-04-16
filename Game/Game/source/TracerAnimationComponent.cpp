#include"TracerAnimationComponent.h"
#include"AIComponent.h"
#include"AIState.h"
#include"Tracer.h"

TracerAnimationComponent::TracerAnimationComponent(Tracer* owner, int order)
	:AnimationComponent(owner, order)
	, _tOwner(owner)
{
	//�A�j���[�V������o�^

	//���f���̓ǂݍ���
	_tOwner->LoadModel("res/Chara/Tracer/tracer_5.mv1");

	//�A�j���[�V�����̓ǂݍ���
	LoadAnimation("Trace", "mo_movetracer_01", 0);
	LoadAnimation("Rush", "mo_attacktracer_01", 0);
	LoadAnimation("Death", "mo_deathtracer_01", 1);

	//�A�j���[�V�����̕����Ԃ�ݒ�
	_closeMaxTime = 12.f;

}

TracerAnimationComponent::~TracerAnimationComponent() {

}

bool TracerAnimationComponent::Process() {

	//�A�j���[�V�����̑J�ڏ������L�q
	std::string name = _tOwner->GetAI()->GetCurrentState()->GetName();

	if(name == "Trace") {
		ChangeAnimation("Trace");
	}
	else if(name == "Rush") {
		ChangeAnimation("Rush");
	}
	else if(name == "Death") {
		ChangeAnimation("Death");
	}

	AnimationComponent::Process();

	return true;
}