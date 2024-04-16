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

	bool _isAttack;							//攻撃中かどうか	

	class CharaBase* _equippedChara;		//装備しているキャラ

	std::string _frameName;					//装備しているキャラのどのフレームに、移動するか

	class FrameComponent* _frame;			//フレームの当たり判定を行うコンポーネント

};