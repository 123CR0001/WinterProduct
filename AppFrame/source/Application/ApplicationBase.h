
#include "dxlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../Mode//ModeServer.h"
#include "../GamePad//XGamePad.h"
#include "../Collision//Collision.h"
#include"../Math/Navi.h"
#include"../Math/Json.h"
#include"../ResourceServer/ResourceServer.h"

// EffekseerForDXLib.hをインクルードします。
#include "EffekseerForDXLib.h"

class ApplicationBase
{
public:
	ApplicationBase();
	virtual ~ApplicationBase();

	virtual bool Initialize(HINSTANCE hInstance);
	virtual bool Terminate();
	virtual bool Input();
	virtual bool Process();
	virtual bool Render();

	virtual bool AppWindowed() { return true; }
	virtual int DispSizeW() { return 1920; }
	virtual int DispSizeH() { return 1080; }

	static	ApplicationBase	*GetInstance() { return _lpInstance; }
	virtual int GetKey() { return _gKey; }
	virtual int GetTrg() { return _gTrg; }

	int GetMouseInputKey()const { return _mouse_key; }
	int GetMouseInputTrg()const { return _mouse_trg; }
	int GetMouseInputRel()const { return _mouse_rel; }
	int GetMouseX()const { return _mouse_x; }
	int GetMouseY()const { return _mouse_y; }
	int GetOldMouseX()const { return old_mouse_x; }
	int GetOldMouseY()const { return old_mouse_y; }

	XGamePad* GetPad()const { return _pad; }

	bool IsGaneEnd()const { return _isGameEnd; }
	void GameEnd() { _isGameEnd = true; }
protected:
	static	ApplicationBase	*_lpInstance;

	int		_gKey, _gTrg;

	ModeServer* _serverMode;

private:
	XGamePad* _pad;
	int _mouse_key;
	int _mouse_trg;
	int _mouse_rel;

	int _mouse_x;
	int _mouse_y;

	int old_mouse_x;
	int old_mouse_y;

	bool _isGameEnd;
};
