#pragma once
#include"ObjectBase.h"
class WeaponBase :public ObjectBase{
public:
	WeaponBase(class CharaBase* chara);
	virtual ~WeaponBase();

	bool Initialize()override;
	bool Process()override;
	bool Render()override;

	//攻撃処理のオンオフ
	void OnAttack() { _isAttack = true; }
	void OffAttack() { _isAttack = false; }

	bool IsHit(class CharaBase* chara);
protected:

	//攻撃処理をするか
	bool _isAttack;

	//自分を装備しているキャラ
	class CharaBase* _equippedChara;

	//デバッグ系
	static bool _isAttackCollisionView;
};