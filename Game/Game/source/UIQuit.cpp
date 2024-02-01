
#include "UIQuit.h"
#include "ModeEndConfirm.h"

UIQuit::UIQuit() {
}

int UIQuit::Selected() {
	ModeServer::GetInstance()->Add(new ModeEndConfirm(), 100, "confirm");
	return 0;
}