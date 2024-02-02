#include "Siren.h"
#include"ObjectServer.h"
#include"ModeGame.h"
#include"PhysWorld.h"
#include"appframe.h"
#include"Player.h"
#include"SoundComponent.h"
#include<algorithm>

int Siren::_maxInterval = 360;
float Siren::_volumeSize = 500.f;
float Siren::_playerDist = 100.f;

Siren::Siren(ObjectServer* server)
	:ObjectBase(server)
	, _interval(0) 
{
	LoadModel("res/Object/Siren.mv1");
}

Siren::~Siren(){}

bool Siren::Initialize() {
	ObjectBase::Initialize();

	GetObjectServer()->GetSirens().emplace_back(this);

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
		Vector3D::LengthSquare(GetObjectServer()->GetPlayer()->GetPos(), _pos) < _playerDist * _playerDist&&
		GetObjectServer()->GetGame()->GetPad()->GetTrgButton() & INPUT_X) {

		_interval = 360;

		//真下のナビゲーションメッシュから、SoundComponentを生成すると、構築する隣接情報が増えるため
		MV1_COLL_RESULT_POLY result = MV1CollCheck_Line(
			GetObjectServer()->GetNavigationHandle(), 
			GetObjectServer()->GetNavigationAttachIndex(),
			DxConverter::VecToDx(_pos - Vector3D(0.f, 100.f, 0.f)),
			DxConverter::VecToDx(_pos + Vector3D(0.f, 100.f, 0.f))
		);

		new SoundComponent(this, DxConverter::DxToVec(result.HitPosition), _volumeSize);
	}
	return true;
}
