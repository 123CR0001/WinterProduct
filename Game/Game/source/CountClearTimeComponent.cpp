#include"CountClearTimeComponent.h"
#include"ObjectBase.h"
#include"ModeGame.h"
#include"ObjectServer.h"
#include"ClearData.h"
CountClearTimeComponent::CountClearTimeComponent(ObjectBase* owner)
	:Component(owner)
	,_frameCnt(0)
{

}

CountClearTimeComponent::~CountClearTimeComponent() {
	//ApplicationGlobal ResultDataのclearTimeに代入
	GetOwner()->GetObjectServer()->GetGame()->GetClearData()->clearSecondTime = static_cast<float>(_frameCnt / 60); //単位:秒
}

bool CountClearTimeComponent::Process() {
	_frameCnt++;
	return true;
}