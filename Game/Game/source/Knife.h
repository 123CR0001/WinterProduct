#pragma once
#include"WeaponBase.h"

class Knife :public WeaponBase {
public:
	Knife(class CharaBase* chara) :WeaponBase(chara) { };
	bool Initialize()override;
	bool Process()override;
protected:

};