#pragma once
#include"UI.h"

class UIDetectionLevel :public UI {
public:
	UIDetectionLevel(class ObjectServer* server);
	~UIDetectionLevel();

	bool Process()override;
	bool Draw()override;

private:

	int _gaugeHandle;
	int _gaugeBgHandle;
	class SpriteText* _text;
	class ObjectServer* _server;
};