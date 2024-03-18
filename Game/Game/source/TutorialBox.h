#pragma once
#include"EventBox.h"

class TutorialBox :public EventBox {
public:
	TutorialBox(class ObjectServer* server,std::string name);
	~TutorialBox();

	bool Process()override;
};