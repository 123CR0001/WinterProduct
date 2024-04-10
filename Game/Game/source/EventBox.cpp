#include"EventBox.h"
#include"ObjectServer.h"
#include"FrameComponent.h"
#include"PhysWorld.h"
#include"Player.h"

EventBox::EventBox(ObjectServer* server)
	:ObjectBase(server)
	,_frameCom(NEW FrameComponent(this))
{
	//�����o���t���[������폜
	_frameCom->DeletePhysWorldThis();

	//���f���̓ǂݍ���
	LoadModel("res/Object/box.mv1");
}

EventBox::~EventBox() {

}

bool EventBox::Initialize() {
	return true;
}

bool EventBox::Process() {
	ObjectBase::Process();
	return true;
}

