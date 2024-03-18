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
	unsigned int _elapsedTime;
	unsigned int _spawnTime;

	std::vector<Vector3>_playerPoints;

	unsigned int _frameCnt;
	const unsigned int _interval;
};