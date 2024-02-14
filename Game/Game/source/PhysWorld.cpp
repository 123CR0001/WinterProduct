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

bool PhysWorld::IsHear(ObjectBase* object,Vector3D* pos) {
	for (auto iter = _sounds.begin(); iter != _sounds.end(); ++iter) {
		if ((*iter)->IsHear(object->GetPos())) {
			if (pos) { *pos = (*iter)->GetPos(); }
			return true;
		}
	}
	return false;
}