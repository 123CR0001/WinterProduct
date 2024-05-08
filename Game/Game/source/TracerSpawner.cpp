#include"TracerSpawner.h"
#include"ObjectServer.h"

#include"Tracer.h"
#include"Player.h"

TracerSpawner::TracerSpawner(ObjectServer* owner)
	:ObjectBase(owner)
	,_elapsedTime(0)
	,_spawnTime(10800)
	//プレイヤーの最初の座標を取得したいから
	,_frameCnt(3)
	,_interval(3)
{

}

TracerSpawner::~TracerSpawner() {

}

bool TracerSpawner::Initialize() {
	ObjectBase::Initialize();
	return true;
}

bool TracerSpawner::Terminate() {
	return true;
}

bool TracerSpawner::Process() {
	//一定時間経過したら、トレーサーをスポーン
	if (_elapsedTime == _spawnTime) {
		NEW Tracer(this);
		//一回きりでいいので、自身を削除
		GetObjectServer()->DeleteObject(this);
	}

	//経過時間をカウント
	_elapsedTime++;

	//プレイヤーの座標を登録
	if (_frameCnt == _interval) {
		_frameCnt = 0;
		_playerPoints.emplace_back(GetObjectServer()->GetPlayer()->GetPos());
	}

	//フレームカウントを進める
	_frameCnt++;


	return true;
}

bool TracerSpawner::Render() {
	return true;
}