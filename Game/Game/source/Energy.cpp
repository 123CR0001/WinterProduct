#include"Energy.h"
#include"ObjectServer.h"
#include"Player.h"
#include"CapsuleComponent.h"
#include"ModeGame.h"

Energy::Energy(ObjectServer* server)
	:ObjectBase(server,false,"Energy")
	,_radius(30.f)
{
	//_energyCount‚ð‘‚â‚·
	server->GetGame()->IncrementEnergyCount();
}

Energy::~Energy() {

}

bool Energy::Process() {

	auto player = GetObjectServer()->GetPlayer();

	if(Collision::Intersection(player->GetCapsuleComponent()->GetCapsule(), Sphere(_pos, _radius))) {
		//_energyCount‚ðŒ¸‚ç‚·
		GetObjectServer()->GetGame()->DecremetEnergyCOunt();
		GetObjectServer()->DeleteObject(this);
	}

	return true;
}

bool Energy::Render() {

	DrawSphere3D(DxConverter::VecToDx(_pos), _radius, 10, GetColor(0, 0, 255), GetColor(0, 0, 255), TRUE);

	return true;
}