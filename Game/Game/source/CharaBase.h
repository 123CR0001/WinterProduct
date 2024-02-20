#pragma once
#include"ObjectBase.h"

class CharaBase :public ObjectBase{
public :
	CharaBase(class ObjectServer* server,std::string name);
	virtual ~CharaBase();

	bool Initialize()override;
	bool Terminate()override;
	bool Render()override;

	ObjectBase* GetOnObject()const { return _onObj; }

	class MoveComponent* GetMoveComponent()const { return _moveCom; }

	//あらかじめ決めておいたstring型で派生先のキャラクタークラスの状態を変更する
	//モーションデータによるアニメーションの切り替えに使用するので、必ず各派生先で定義する 
	virtual bool ChangeState(std::string stateName);

protected:

	bool	_isStand;		//オブジェクトの上に立っているか

	class MoveComponent* _moveCom;

	//乗っているオブジェクトのアドレスを格納 乗っているオブジェクトが移動して一緒に移動するときに
	ObjectBase* _onObj;
};

