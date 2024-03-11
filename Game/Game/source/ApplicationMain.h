#pragma once
#include "appframe.h"

constexpr int MAX_SCREEN_WIDTH = 1920;
constexpr int MAX_SCREEN_HEIGHT = 1080;

class ApplicationMain : public ApplicationBase
{
	typedef ApplicationBase base;
public:
	virtual bool Initialize(HINSTANCE hInstance);
	virtual bool Terminate();
	virtual bool Input();
	virtual bool Process();
	virtual bool Render();

	virtual bool AppWindowed() { return true; }
	virtual int DispSizeW() { 
#ifdef _DEBUG
		return 1280;
#endif 
		return MAX_SCREEN_WIDTH;
	 }
	virtual int DispSizeH() {
#ifdef _DEBUG
		return 720;
#endif 
		return MAX_SCREEN_HEIGHT;
	}

protected:

}; 
