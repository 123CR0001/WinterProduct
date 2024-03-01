
#include "appframe.h"


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
		return 1920;
	 }
	virtual int DispSizeH() {
#ifdef _DEBUG
		return 720;
#endif 
		return 1080;
	}

protected:

}; 
