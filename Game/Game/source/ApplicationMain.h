#pragma once
#include "appframe.h"

constexpr int MAX_SCREEN_WIDTH = 1920;
constexpr int MAX_SCREEN_HEIGHT = 1080;

#if _DEBUG
constexpr int SCREEN_WIDTH = 1280;
constexpr int SCREEN_HEIGHT = 720;
#else
constexpr int SCREEN_WIDTH = 1920;
constexpr int SCREEN_HEIGHT = 1080;
#endif


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
		return SCREEN_WIDTH;
	}
	virtual int DispSizeH() {
		return SCREEN_HEIGHT;
	}

protected:

};
