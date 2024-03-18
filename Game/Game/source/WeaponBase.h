#pragma once
#include"ObjectBase.h"
class WeaponBase :public ObjectBase{
public:
	WeaponBase(class CharaBase* chara);
	virtual ~WeaponBase();

	bool Initialize()override;
	bool Process()override;
	bool Render()override;

	//�U�������̃I���I�t
	void OnAttack() { _isAttack = true; }
	void OffAttack() { _isAttack = false; }

protected:

	//�U�����������邩
	bool _isAttack;

	//�����𑕔����Ă���L����
	class CharaBase* _equippedChara;

	//�t���[���̓����蔻����s���R���|�[�l���g
	class FrameComponent* _frame;

};