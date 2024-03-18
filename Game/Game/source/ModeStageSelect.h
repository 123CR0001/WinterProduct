#pragma once
#include"appframe.h"

class ModeStageSelect :public ModeBase {
	
	bool Initialize()override;
	bool Terminate()override;
	bool Process()override;
	bool Render()override;

private:
	class ButtonServer* _buttonServer;
	

};