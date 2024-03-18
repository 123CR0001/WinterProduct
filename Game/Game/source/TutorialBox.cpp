#include"TutorialBox.h"
#include"Player.h"
#include"ObjectServer.h"
#include"SpriteText.h"
#include"TransformAnimation.h"
#include"PhysWorld.h"
#include"FrameComponent.h"
#include"ModeGame.h"
#include"ModeTutorial.h"

TutorialBox::TutorialBox(ObjectServer* server,std::string name)
	:EventBox(server)
{
	_name = name;
}

TutorialBox::~TutorialBox(){}

bool TutorialBox::Process() {
	EventBox::Process();

	if(_frameCom->GetBeginOverlap() == GetObjectServer()->GetPlayer()) {
		ModeServer::GetInstance()->Add(NEW ModeTutorial(_name), 1000, "Tutorial");
	}

	return true;
}