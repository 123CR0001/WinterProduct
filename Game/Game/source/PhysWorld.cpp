#include"PhysWorld.h"
#include"ObjectServer.h"
#include"ObjectBase.h"
#include"SoundComponent.h"

PhysWorld::PhysWorld(ObjectServer* server)
	:_server(server)
{

}

PhysWorld::~PhysWorld() {

}

bool PhysWorld::IsHear(ObjectBase* object,Vector3* pos) {

	//obejctとの距離を比較して、音が聞こえているかどうかを判定
	for (auto iter = _sounds.begin(); iter != _sounds.end(); ++iter) {

		//音が聞こえているかどうか
		if ((*iter)->IsHear(object->GetPos())) {

			//音の位置を取得
			if (pos) { *pos = (*iter)->GetPos(); }

			//音が聞こえている
			return true;
		}
	}

	//音が聞こえていない
	return false;
}