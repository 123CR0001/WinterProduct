#pragma once

#include "appframe.h"
#include "UIBase.h"
#include <list>

class UIServer {
	typedef std::list<UIBase*>	lstUIBase;

public:
	UIServer();
	~UIServer();

public:
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();

public:
	void	Add(UIBase* uiBase, void* param, int cg, float x, float y, float w, float h, int layer, std::string name);
	void	Clear();
	UIBase*	Search(std::string name);
	void SelectPosAdjustment(std::string selectName, std::string get, int shiftValueX, int shiftValueY);
	void HideDisplay(UIBase* uiBase, bool isHide);
	bool IsRegistering(std::string);
	void Del(std::string);

	static bool uiSort(const UIBase* x, const UIBase* y) {
		return x->_layer < y->_layer;
	}

	float		ConvertXPosToDispSize(float pos);
	float		ConvertYPosToDispSize(float pos);

public:
	lstUIBase	_vUI;	// UIƒŠƒXƒg
	UIBase*		_skipProcessUI;
};

