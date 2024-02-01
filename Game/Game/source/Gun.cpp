#include"Gun.h"
#include"CharaBase.h"

Gun::Gun(CharaBase* chara)
	:WeaponBase(chara)
{

}

Gun::~Gun(){}

bool Gun::Initialize() {
	WeaponBase::Initialize();

	if (_isAttack) {

	}

	return true;
}

bool Gun::Process() {
	return true;
}

bool Gun::Render() {
	return true;
}