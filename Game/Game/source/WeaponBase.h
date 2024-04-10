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

	void SetFrameName(const std::string name) { _frameName = name; }

protected:

	//攻撃処理をするか
	bool _isAttack;

	//自分を装備しているキャラ
	class CharaBase* _equippedChara;

	//装備しているキャラのどのフレームに、移動するか
	std::string _frameName;

	//フレームの当たり判定を行うコンポーネント
	class FrameComponent* _frame;

};