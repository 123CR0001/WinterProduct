
#include "ModeGameUI.h"
#include "ModeGame.h"

#include "UIServer.h"
#include "UILightsTimer.h"

ModeGameUI::ModeGameUI() {
}

ModeGameUI::~ModeGameUI() {
		
}

bool ModeGameUI::Initialize() {
	_uiServer = NEW UIServer();

	_uiServer->Add(NEW UILightsTimer(), nullptr, 0, 0, 930, 600, 150, 100, "lightsOutTimer");

	return false;
}

bool ModeGameUI::Terminate() {
	delete _uiServer;
	return false;
}

bool ModeGameUI::Process() {
	_uiServer->Process();



	return false;
}

bool ModeGameUI::Render() {
	_uiServer->Render();
	return false;
}
