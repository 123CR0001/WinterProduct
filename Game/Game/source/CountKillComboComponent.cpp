#include"CountKillComboComponent.h"
#include"ObjectBase.h"
#include"ObjectServer.h"
#include"ModeGame.h"
#include"ClearData.h"

int CountKillComboComponent::_frameCnt = 0;
int CountKillComboComponent::_comboCnt = 0;

CountKillComboComponent::CountKillComboComponent(ObjectBase* owner)
	:Component(owner) 
{

}

CountKillComboComponent::~CountKillComboComponent() {

}

bool CountKillComboComponent::Process() {
	//ダメージを受けた
	//コンボのカウントを開始
	if(_owner->GetDamageData().isDamage) {
		_frameCnt = 30;
		_comboCnt++;
		if(GetOwner()->GetObjectServer()->GetGame()->GetClearData()->maxCombo < _comboCnt) {
			GetOwner()->GetObjectServer()->GetGame()->GetClearData()->maxCombo = _comboCnt;
		}
	}

	//コンボのカウントが終了
	if(_frameCnt <= 0) {
		_comboCnt = 0;
	}

	//カウントを減らす
	_frameCnt--;
	

	return true;
}