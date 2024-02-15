#pragma once

#include "appframe.h"

class UIBase {
public:
	void*	_param;
	int		_cg;

	float		_x, _y;
	float		_w, _h;
	int		_layer;
	std::string		_uiName;

	int		_selectNum;
	bool	_bView;

	// スタート地点
	float _initPosX, _initPosY;
	float _endPosX, _endPosY;

	int _cnt;
	int _frame;

private:
	class UIServer* _server;

public:
	UIBase();

public:
	virtual void Terminate();
	virtual void Process();
	virtual void Render();

public:
	virtual int Selected();
	virtual void SetSlideUIPosition(float initPosX, float initPosY, float endPosX, float endPosY, int frame);
	bool IsSlide();

public:
	virtual void	SetPos(float posX, float posY) { _x = posX; _y = posY; }
	virtual float	GetPosX() { return _x; }
	virtual float	GetPosY() { return _y; }
	class UIServer* GetUIServer()const { return _server; }
};
