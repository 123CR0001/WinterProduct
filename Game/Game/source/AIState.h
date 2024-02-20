#pragma once

class AIState {
public:
	AIState(class AIComponent* owner);
	virtual ~AIState();
	virtual bool Process() = 0;

	//状態の切り替えの際に自クラスになった場合にする処理
	virtual void OnEnter() = 0;

	//状態の切り替えの際に自クラスから別クラスに遷移する場合にする処理
	virtual void OnExist() = 0;

	//固有の名前を返す
	virtual const char* GetName() = 0;

protected:
	class AIComponent* _owner;
};
