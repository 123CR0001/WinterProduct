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

	//obejct‚Æ‚Ì‹——£‚ð”äŠr‚µ‚ÄA‰¹‚ª•·‚±‚¦‚Ä‚¢‚é‚©‚Ç‚¤‚©‚ð”»’è
	for (auto iter = _sounds.begin(); iter != _sounds.end(); ++iter) {

		//‰¹‚ª•·‚±‚¦‚Ä‚¢‚é‚©‚Ç‚¤‚©
		if ((*iter)->IsHear(object->GetPos())) {

			//‰¹‚ÌˆÊ’u‚ðŽæ“¾
			if (pos) { *pos = (*iter)->GetPos(); }

			//‰¹‚ª•·‚±‚¦‚Ä‚¢‚é
			return true;
		}
	}

	//‰¹‚ª•·‚±‚¦‚Ä‚¢‚È‚¢
	return false;
}