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

	//obejct�Ƃ̋������r���āA�����������Ă��邩�ǂ����𔻒�
	for (auto iter = _sounds.begin(); iter != _sounds.end(); ++iter) {

		//�����������Ă��邩�ǂ���
		if ((*iter)->IsHear(object->GetPos())) {

			//���̈ʒu���擾
			if (pos) { *pos = (*iter)->GetPos(); }

			//�����������Ă���
			return true;
		}
	}

	//�����������Ă��Ȃ�
	return false;
}