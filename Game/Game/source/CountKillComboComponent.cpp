#include"CountKillComboComponent.h"
#include"ObjectBase.h"
#include"ObjectServer.h"
#include"ModeGame.h"
#include"ResultData.h"

int CountKillComboComponent::_frameCnt = 0;
int CountKillComboComponent::_comboCnt = 0;

CountKillComboComponent::CountKillComboComponent(ObjectBase* owner)
	:Component(owner) 
{

}

CountKillComboComponent::~CountKillComboComponent() {

}

bool CountKillComboComponent::Process() {
	//�_���[�W���󂯂�
	//�R���{�̃J�E���g���J�n
	if(_owner->GetDamageData().isDamage) {
		_frameCnt = 30;
		_comboCnt++;
		if(GetOwner()->GetObjectServer()->GetGame()->GetResultData()->maxCombo < _comboCnt) {
			GetOwner()->GetObjectServer()->GetGame()->GetResultData()->maxCombo = _comboCnt;
		}
	}

	if(_frameCnt <= 0) {

		_comboCnt = 0;
	}

	_frameCnt--;
	

	return true;
}