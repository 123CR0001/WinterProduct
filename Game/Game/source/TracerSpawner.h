#pragma once
#include"ObjectBase.h"

class TracerSpawner :public ObjectBase {
public:
	TracerSpawner(class ObjectServer* server);
	~TracerSpawner();

	bool Initialize()override;
	bool Terminate()override;
	bool Process()override;
	bool Render()override;

	std::vector<Vector3>& GetPoints() { return _playerPoints; }

private:
	unsigned int _elapsedTime;				//経過時間
	unsigned int _spawnTime;				//トレーサーの生成時間

	std::vector<Vector3>_playerPoints;

	//プレイヤーの座標を記録する間隔
	unsigned int _frameCnt;					//フレームカウント
	const unsigned int _interval;			//間隔	
};