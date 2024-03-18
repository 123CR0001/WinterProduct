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

protected:

	//攻撃処理をするか
	bool _isAttack;

	//自分を装備しているキャラ
	class CharaBase* _equippedChara;

	//フレームの当たり判定を行うコンポーネント
	class FrameComponent* _frame;

};