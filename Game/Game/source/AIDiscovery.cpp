#include"AIDiscovery.h"
#include"AIComponent.h"

AIDiscovery::AIDiscovery(AIComponent* owner)
	:AIState(owner)
	, _frameCnt(0)
{

}

AIDiscovery::~AIDiscovery() {

}

void AIDiscovery::OnEnter() {
	_frameCnt = 0;
}

void AIDiscovery::OnExist() {

}

bool AIDiscovery::Process() {

	_frameCnt++;
	//60ƒtƒŒ[ƒ€Œo‰ß‚µ‚½‚çŽŸ‚Ìó‘Ô‚Ö
	if(_frameCnt > 60) {
		_owner->ChangeState("MoveTo");
	}
	return true;
}