#include"CreateAfterImageComponent.h"
#include"ObjectBase.h"
#include"CharaBase.h"
#include"AfterImage.h"
#include"AnimationConmponent.h"

CreateAfterImageComponent::CreateAfterImageComponent(CharaBase* owner)
	:Component(owner,10000)
	,_chara(owner)
	,_maxCnt(10)
	,_frameCnt(0)
{

}

CreateAfterImageComponent::~CreateAfterImageComponent() {

}

bool CreateAfterImageComponent::Process() {

	if (_frameCnt == _maxCnt) {
		NEW AfterImage(_chara->GetAnimationComponent());
		_frameCnt = 0;
	}

	_frameCnt++;

	return true;
}