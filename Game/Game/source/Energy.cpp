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
	,_text(std::make_unique<SpriteTextFlipAnimation>(8,true))
{
	//_energyCountを増やす
	server->GetGame()->IncrementEnergyCount();
	_text->LoadDivText("res/UI/Game/ef_energy.png", 20, 20, 1, 100, 100);
	_text->SetAlpha(0.f);
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
	SetDrawBlendMode(DX_BLENDMODE_ADD, 255);

	DrawBillboard3D(
		DxConverter::VecToDx(_pos + Vector3(0.f,10.f,0.f)),
		0.5f,
		0.5f,
		100.f,
		0.f,
		_text->GetHandle(),
		FALSE
	);

	_text->Draw();
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	return true;
}