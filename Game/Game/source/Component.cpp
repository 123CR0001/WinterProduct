#include "Component.h"
#include"ObjectBase.h"

Component::Component(ObjectBase* owner,int order)
	:_owner(owner) 
	,_order(order)
{
	_owner->AddComponent(this);
}

Component::~Component(){

}

bool Component::Process(){
	return true;
}