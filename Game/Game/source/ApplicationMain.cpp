
#include "AppFrame.h"
#include "ApplicationMain.h"
#include "ModeTitle.h"
#include "ModeSoundLayer.h"

// ŽÀ‘Ì
ApplicationMain				g_oApplicationMain;

bool ApplicationMain::Initialize(HINSTANCE hInstance) {
	if (!base::Initialize(hInstance)) { return false; }

	// ƒ‚[ƒh‚Ì“o˜^
	ModeServer::GetInstance()->Add(new ModeTitle(), 1, "title");
	ModeServer::GetInstance()->Add(new ModeSoundLayer(), 99999, "SoundLayer");

	return true;
}

bool ApplicationMain::Terminate() {
	base::Terminate();
	return true;
}


bool ApplicationMain::Input() {
	base::Input();
	return true;
}

bool ApplicationMain::Process() {
	base::Process();
	return true;
}

bool ApplicationMain::Render() {
	base::Render();
	return true;
}

