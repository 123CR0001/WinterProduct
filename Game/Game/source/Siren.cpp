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
float Siren::_playerDist = 200.f;

Siren::Siren(ObjectServer* server)
	:ObjectBase(server,false,"Siren")
	, _interval(0) 
	,_SEName("SE_07")
	,_volumeSize(400.f)
{

}

Siren::~Siren(){}

bool Siren::Initialize() {
	ObjectBase::Initialize();

	GetObjectServer()->GetSirens().emplace_back(this);

	//拡大
	_scale = Vector3(2.0, 2.0, 2.0);

	//モデルの読み込み
	LoadModel("res/Object/siren/siren.mv1");

	//当たり判定用のフレームは描画しない
	MV1SetFrameVisible(_handle, MV1SearchFrame(_handle, "UCX_Keihouki"), FALSE);

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

	//インターバルを減らす
	if (_interval > 0)
	{
		_interval--;
	}

	//プレイヤーが近くにいて、ボタンが押されたら
	if (_interval <= 0 &&
		Vector3::LengthSquare(GetObjectServer()->GetPlayer()->GetPos(), _pos) < _playerDist * _playerDist &&
		GetObjectServer()->GetGame()->GetPad()->GetTrgButton() & INPUT_B
		) {

		//インターバルを設定
		_interval = 360;

		Vector3 pos = _pos;

		pos += GetForward() * 50.f;
	
		//サウンドコンポーネントを追加
		new SoundComponent(this, pos, _volumeSize);

		//SEを再生
		auto snd = gGlobal._sndServer.Get(_SEName);
		if(snd) {
			if(!snd->IsPlay()) {
				snd->Play();
			}
		}

	}

	return true;
}

bool Siren::Render() {
	ObjectBase::Render();

	//Vector3 pos = _pos;

	//pos += GetForward() * 50.f;

	//DrawSphere3D(DxConverter::VecToDx(pos), 30.f, 10, GetColor(255, 255, 255), GetColor(255, 255, 255), TRUE);

	return true;
}