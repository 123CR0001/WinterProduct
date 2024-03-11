#pragma once
#include"UI.h"
#include<vector>
#include"Dxlib.h"

class UIVision :public UI {
public:
	UIVision(class ObjectServer* server);
	~UIVision();

	bool Process()override;
	bool Draw()override;

private:

	int _handle;

	std::vector<unsigned short>_versNums;
	std::vector<VERTEX3D>_vers;

	class ObjectServer* _server;
};