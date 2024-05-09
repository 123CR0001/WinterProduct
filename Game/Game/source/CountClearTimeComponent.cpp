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
	//ApplicationGlobal ResultData��clearTime�ɑ��
	GetOwner()->GetObjectServer()->GetGame()->GetClearData()->clearSecondTime = _frameCnt / 60; //�P��:�b
}

bool CountClearTimeComponent::Process() {
	_frameCnt++;
	return true;
}