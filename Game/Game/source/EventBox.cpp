#include"EventBox.h"
#include"ObjectServer.h"
#include"FrameComponent.h"
#include"PhysWorld.h"
#include"Player.h"

EventBox::EventBox(ObjectServer* server)
	:ObjectBase(server)
	,_frameCom(NEW FrameComponent(this))
{
	//押し出すフレームから削除
	_frameCom->DeletePhysWorldThis();

	//モデルの読み込み
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

