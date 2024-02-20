#pragma once
#include"ObjectBase.h"

class TraserSpawner :public ObjectBase {
public:
	TraserSpawner(class ObjectServer* server);
	~TraserSpawner();

	bool Initialize()override;
	bool Terminate()override;
	bool Process()override;
	bool Render()override;

	std::vector<Vector3D>& GetPoints() { return _playerPoints; }

private:
	unsigned int _elapsedTime;
	unsigned int _spawnTime;

	std::vector<Vector3D>_playerPoints;

	unsigned int _frameCnt;
	const unsigned int _interval;
};