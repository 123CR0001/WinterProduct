#pragma once
#include"EventBox.h"

class TutorialDecoy :public EventBox {
public:
	TutorialDecoy(class ObjectServer* server);
	~TutorialDecoy();

	bool Process()override;
private:
	class SpriteText* _text;
};