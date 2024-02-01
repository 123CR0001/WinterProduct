#pragma once
#include"WeaponBase.h"

class Gun :public WeaponBase {
public:
	Gun(class CharaBase* chara);
	virtual ~Gun();

	bool Initialize()override;
	bool Process()override;
	bool Render()override;
};

class Bullet :public WeaponBase {

};