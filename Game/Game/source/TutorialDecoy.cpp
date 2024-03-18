#include"TutorialDecoy.h"
#include"Player.h"
#include"ObjectServer.h"
#include"SpriteText.h"
#include"TransformAnimation.h"
#include"PhysWorld.h"
#include"FrameComponent.h"
#include"ModeGame.h"

TutorialDecoy::TutorialDecoy(ObjectServer* server)
	:EventBox(server)
	,_text(NEW SpriteText())
{

}

TutorialDecoy::~TutorialDecoy(){}

bool TutorialDecoy::Process() {

	if(_frameCom->GetBeginOverlap() == GetObjectServer()->GetPlayer()) {

	}

	return true;
}