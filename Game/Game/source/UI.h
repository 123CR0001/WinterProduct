#pragma once

class UI {
public:
	UI(int drawOrder = 100) 
		:_drawOrder(drawOrder)
	{}
	virtual ~UI(){}

	virtual const char* GetName() = 0;

	virtual bool Process() = 0;
	virtual bool Draw() = 0;

	int GetDrawOrder()const { return _drawOrder; }

protected:
	int _drawOrder;		//•`‰æ‡
};
