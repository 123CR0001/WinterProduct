#pragma once

class Component {
public:
	Component(class ObjectBase* owner, int order = 100);
	virtual ~Component();

	virtual bool Process();

	class ObjectBase* GetOwner()const { return _owner; }
	int GetOrder()const { return _order; }

protected:
	class ObjectBase* _owner;
	int _order;
};