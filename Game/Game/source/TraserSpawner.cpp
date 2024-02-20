#include"TraserSpawner.h"
#include"ObjectServer.h"

#include"Traser.h"
#include"Player.h"

TraserSpawner::TraserSpawner(ObjectServer* owner)
	:ObjectBase(owner)
	,_elapsedTime(0)
	,_spawnTime(300)
	//プレイヤーの最初の座標を取得したいから
	,_frameCnt(30)
	,_interval(30)
{

}

TraserSpawner::~TraserSpawner() {

}

bool TraserSpawner::Initialize() {
	return true;
}

bool TraserSpawner::Terminate() {
	return true;
}

bool TraserSpawner::Process() {
	//一定時間経過したら、トレーサーをスポーン
	if (_elapsedTime == _spawnTime) {
		NEW Traser(this);
		//一回きりでいいので、自身を削除
		GetObjectServer()->DeleteObject(this);
	}

	_elapsedTime++;


	if (_frameCnt == _interval) {
		_frameCnt = 0;
		_playerPoints.emplace_back(GetObjectServer()->GetPlayer()->GetPos());
	}
	_frameCnt++;


	return true;
}

bool TraserSpawner::Render() {
	return true;
}