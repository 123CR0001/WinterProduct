#include"TracerAnimationComponent.h"
#include"AIComponent.h"
#include"AIState.h"
#include"Tracer.h"

TracerAnimationComponent::TracerAnimationComponent(Tracer* owner, int order)
	:AnimationComponent(owner, order)
	, _tOwner(owner)
{
	//アニメーションを登録

	_tOwner->LoadModel("res/Chara/Tracer/tracer_5.mv1");
	LoadAnimation("Trace", "mo_movetracer_01", 0);
	LoadAnimation("Rush", "mo_attacktracer_01", 0);
	LoadAnimation("Death", "mo_deathtracer_01", 1);
	_closeMaxTime = 12.f;

}

TracerAnimationComponent::~TracerAnimationComponent() {

}

bool TracerAnimationComponent::Process() {
	AnimationComponent::Process();

	//アニメーションの遷移条件を記述


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
	return true;
}