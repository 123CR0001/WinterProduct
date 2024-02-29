#include "Siren.h"
#include"ObjectServer.h"
#include"ModeGame.h"
#include"PhysWorld.h"
#include"appframe.h"
#include"Player.h"
#include"SoundComponent.h"
#include"ApplicationGlobal.h"
#include<algorithm>

int Siren::_maxInterval = 360;
float Siren::_volumeSize = 700.f;
float Siren::_playerDist = 200.f;

Siren::Siren(ObjectServer* server)
	:ObjectBase(server)
	, _interval(0) 
{
	LoadModel("res/Object/siren/siren.mv1");
	MV1SetFrameVisible(_handle, MV1SearchFrame(_handle,"UCX_Keihouki"),FALSE);
}

Siren::~Siren(){}

bool Siren::Initialize() {
	ObjectBase::Initialize();

	GetObjectServer()->GetSirens().emplace_back(this);

	_scale = Vector3D(2.0, 2.0, 2.0);

	return true;
}

bool Siren::Terminate() {
	ObjectBase::Terminate();

	auto iter = std::find(GetObjectServer()->GetSirens().begin(), GetObjectServer()->GetSirens().end(), this);

	if (iter != GetObjectServer()->GetSirens().end()) {
		GetObjectServer()->GetSirens().erase(iter);
	}

	return true;
}

bool Siren::Process() {
	ObjectBase::Process();

	if (_interval > 0)
	{
		_interval--;
	}

	if (_interval <= 0 &&
		Vector3D::LengthSquare(GetObjectServer()->GetPlayer()->GetPos(), _pos) < _playerDist * _playerDist &&
		GetObjectServer()->GetGame()->GetPad()->GetTrgButton() & INPUT_B
		) {

		_interval = 360;

		new SoundComponent(this,_pos, _volumeSize);

		gGlobal._sndServer.Get("SE_07")->Play();
	}
	return true;
}
