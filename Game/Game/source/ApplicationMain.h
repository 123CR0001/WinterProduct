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

constexpr float SCREEN_WIDTH_MAG = static_cast<float>(SCREEN_WIDTH) / static_cast<float>(MAX_SCREEN_WIDTH);
constexpr float SCREEN_HEIGHT_MAG = static_cast<float>(SCREEN_HEIGHT) / static_cast<float>(MAX_SCREEN_HEIGHT);


class ApplicationMain : public ApplicationBase
{
	typedef ApplicationBase base;
public:
	virtual bool BeforeDXLib_Init();

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
