#include"Energy.h"
#include"ObjectServer.h"
#include"Player.h"
#include"CapsuleComponent.h"
#include"ModeGame.h"
#include"ModeEffekseer.h"
#include"SpriteTextFlipAnimation.h"
#include"ApplicationGlobal.h"


Energy::Energy(ObjectServer* server)
	:ObjectBase(server,false,"Energy")
	,_radius(30.f)
{
	//_energyCountを増やす
	server->GetGame()->IncrementEnergyCount();

	//モデルの読み込み
	LoadModel("res/Object/energy/energysphere.mv1");

}

Energy::~Energy() {

}

bool Energy::Process() {

	auto player = GetObjectServer()->GetPlayer();

	if(Collision::Intersection(player->GetCapsuleComponent()->GetCapsule(), Sphere(_pos, _radius))) {
		//_energyCountを減らす
		GetObjectServer()->GetGame()->DecremetEnergyCOunt();
		GetObjectServer()->DeleteObject(this);
		GetObjectServer()->GetGame()->GetModeEffekseer()->Play("GetEnergy", _pos, _eulerAngle);

		gGlobal._sndServer.Play("SE_28");
	}

	return true;
}

bool Energy::Render() {

	ObjectBase::Render();

	return true;
}