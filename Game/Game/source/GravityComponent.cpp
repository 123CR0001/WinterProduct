#include"GravityComponent.h"
#include"ObjectBase.h"
#include"ObjectServer.h"
#include"ModeGame.h"
#include"Map.h"

void GravityComponent::Process(ObjectBase* obj) {
	//重力処理
	if (obj->_gravity > -40.f) {
		obj->_gravity -= 1.f;
	}
	obj->_pos.y += obj->_gravity;


	MV1_COLL_RESULT_POLY hit = MV1CollCheck_Line(obj->GetServer()->GetGame()->GetMap()->GetHandleMap(),
		obj->GetServer()->GetGame()->GetMap()->GetCollisionIndex(),
		VAdd(obj->_pos, VGet(0, obj->_colSubY, 0)), VAdd(obj->_pos, VGet(0, 0, 0)));
	if (hit.HitFlag) {
		// 当たった
		// 当たったY位置をキャラ座標にする
		if (obj->_old_pos.y > obj->_pos.y) {
			obj->_pos.y = hit.HitPosition.y - 0.1f;
			obj->_is_stand = true;
		}
		else {
			obj->_pos.y = VSub(hit.HitPosition, VGet(0, obj->_colSubY, 0)).y;
		}
		obj->_gravity = 0;

	}
	else { obj->_is_stand = false; }
	MV1SetPosition(obj->_handle, obj->_pos);
}