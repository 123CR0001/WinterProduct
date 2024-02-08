#pragma once
#include"ObjectBase.h"

class CharaBase :public ObjectBase{
public :
	CharaBase(class ObjectServer* server);
	virtual ~CharaBase();

	bool Initialize()override;
	bool Terminate()override;
	bool Render()override;

	float GetRadian()const { return _radian; }
	float GetCollSubY()const { return _colSubY; }

	void FixPos(); //キャラやオブジェクトの重なりを修正

	ObjectBase* GetOnObject()const { return _onObj; }
	Segment GetCapsuleSegment() {
		Vector3D under_pos(_pos);
		under_pos.y += _radian;
		Vector3D over_pos(under_pos);
		over_pos.y += _colSubY;
		return Segment(under_pos, over_pos);
	}

	class CapsuleComponent* GetCapsuleComponent() { return _capsule; }

	//あらかじめ決めておいたstring型で派生先のキャラクタークラスの状態を変更する
	//モーションデータによるアニメーションの切り替えに使用するので、必ず各派生先で定義する 
	virtual bool ChangeState(std::string stateName);

protected:
	//カプセルの当たり判定
	float _radian;
	float _colSubY;	// コリジョン判定時のY補正(腰位置）.

	bool	_isStand;		//オブジェクトの上に立っているか

	class AnimationComponent* _anim;
	class CapsuleComponent* _capsule;

	//乗っているオブジェクトのアドレスを格納 乗っているオブジェクトが移動して一緒に移動するときに
	ObjectBase* _onObj;

	//モーションデータの何番目を使用するか(キャラの種類は「string」で、モーションは個々のクラス(列挙型クラス)で定義)
	unsigned int _motCnt;

};

