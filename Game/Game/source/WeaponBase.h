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

	bool _isAttack;							//�U�������ǂ���	

	class CharaBase* _equippedChara;		//�������Ă���L����

	std::string _frameName;					//�������Ă���L�����̂ǂ̃t���[���ɁA�ړ����邩

	class FrameComponent* _frame;			//�t���[���̓����蔻����s���R���|�[�l���g

};