#include"CreateAfterImageComponent.h"
#include"ObjectBase.h"
#include"CharaBase.h"
#include"AfterImage.h"
#include"AnimationComponent.h"

CreateAfterImageComponent::CreateAfterImageComponent(AnimationComponent* owner)
	:Component(owner->GetOwner(), 10000)
	,_anim(owner)
	,_maxCnt(10)
	,_frameCnt(0)
{

}

CreateAfterImageComponent::~CreateAfterImageComponent() {

}

bool CreateAfterImageComponent::Process() {

	if (_frameCnt == _maxCnt) {
		NEW AfterImage(_anim);
		_frameCnt = 0;
	}

	_frameCnt++;

	return true;
}