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

	void SetFrameName(const std::string name) { _frameName = name; }

protected:

	//�U�����������邩
	bool _isAttack;

	//�����𑕔����Ă���L����
	class CharaBase* _equippedChara;

	//�������Ă���L�����̂ǂ̃t���[���ɁA�ړ����邩
	std::string _frameName;

	//�t���[���̓����蔻����s���R���|�[�l���g
	class FrameComponent* _frame;

};