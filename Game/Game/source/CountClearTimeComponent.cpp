#include"CountClearTimeComponent.h"
#include"ObjectBase.h"
#include"ModeGame.h"
#include"ObjectServer.h"

CountClearTimeComponent::CountClearTimeComponent(ObjectBase* owner)
	:Component(owner)
	,_frameCnt(0)
{

}

CountClearTimeComponent::~CountClearTimeComponent() {
	//ApplicationGlobal ResultData‚ÌclearTime‚É‘ã“ü
	GetOwner()->GetObjectServer()->GetGame()->GetResultData()->clearSecondTime = static_cast<float>(_frameCnt / 60); //’PˆÊ:•b
}

bool CountClearTimeComponent::Process() {
	_frameCnt++;
	return true;
}